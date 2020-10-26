#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>

typedef struct find_primes_args{
    int upper_limit;
    int *return_val;
} find_primes_args;


static bool in_non_primes(int num,int *non_primes){

    int array_len = non_primes[0];
    // start at 1 since first elem represents array size
    for(int i=1;i<array_len;++i){
        if(non_primes[i]==num){
            return true;
        }
    }
    return false;
}

static int* init_array(){
    int *array = malloc(sizeof(int));
    if(!array){return NULL;}
    // first elements represent size of array
    array[0]=1;
    return array;

}
static void destroy_array(int **int_array){
    if(int_array && *int_array){
        free(*int_array);
        *int_array = NULL;
    }
}

static void append_to_array(int **array,int num){
    (*array)[0]++;
    // memory errors are handled by the caller
    *array = realloc(*array,(*array)[0]*sizeof(int));
    if(*array==NULL){
        destroy_array(array);
        return;
    }
    (*array)[(*array)[0]-1]=num;
}

static void fill_non_primes(int prime,int **non_primes,int upper_limit){
    for(int i=prime+1;i<=upper_limit;++i){
        // the crux of the Sieve of Eratosthenes
        if(i%prime==0){
            append_to_array(non_primes,i);
            if(!non_primes){
                return;
            }
        }
    }
}

static void *find_primes(void *args_untyped) {

    find_primes_args *args = (find_primes_args*)args_untyped;
    int upper_limit = args->upper_limit;
    int *non_primes = init_array();
    int *primes = init_array();
    bool needs_cleanup = false;

    if(non_primes==NULL||primes==NULL){
        destroy_array(&non_primes);
        destroy_array(&primes);
        return NULL;
    }


    for (int i = 2; i < upper_limit; ++i) {
        // if the condition is true, it's a prime
        if (!in_non_primes(i, non_primes)) {
            append_to_array(&primes,i);
            if(primes==NULL){
                needs_cleanup = true;
                break;
            }
            fill_non_primes(i,&non_primes,upper_limit);
            if(non_primes==NULL){
                needs_cleanup = true;
                break;
            }
        }
    }

    if(needs_cleanup){
        //printf("Memory error");
        destroy_array(&primes);
        destroy_array(&non_primes);
        return NULL;
    }

    destroy_array(&non_primes);
    args->return_val=primes;
    return NULL;


}


static PyObject *
find_primes_find_primes(PyObject *self, PyObject *args)
{
    int upper_limit;
    if (!PyArg_ParseTuple(args, "i|:find_primes",&upper_limit)){
        return NULL;
    }


    find_primes_args *fp_args = malloc(sizeof(find_primes_args));
    if(fp_args==NULL){
        PyErr_SetString(PyExc_MemoryError,"Cannot do prime number calculation. Not enough memory");
        return NULL;
    }
    Py_BEGIN_ALLOW_THREADS
    pthread_t thrd;
    fp_args->upper_limit=upper_limit;
    pthread_create(&thrd,NULL,&find_primes,fp_args);
    pthread_join(thrd,NULL);
    Py_END_ALLOW_THREADS
    PyObject *list = PyList_New(fp_args->return_val[0]-1);
    for(int i=1;i<fp_args->return_val[0];++i){
        PyList_SetItem(list,i-1,PyLong_FromLong(fp_args->return_val[i]));
    }
    destroy_array(&(fp_args->return_val));
    free(fp_args);
    return list;
}



static PyMethodDef FindPrimesMethods[] = {
        {"find_primes",  find_primes_find_primes, METH_VARARGS, "Find primes up to the nth given number"},
        {NULL, NULL, 0, NULL}        /* Sentinel */
};

static struct PyModuleDef find_primes_module = {
        PyModuleDef_HEAD_INIT,
        "find_primes",   /* name of module */
        "Exposed a function that calculates primes using multithreading", /* module documentation, may be NULL */
        -1,       /* size of per-interpreter state of the module,
                 or -1 if the module keeps state in global variables. */
        FindPrimesMethods
};

PyMODINIT_FUNC
PyInit_find_primes(void)
{
    return PyModule_Create(&find_primes_module);
}