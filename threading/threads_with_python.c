#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <unistd.h>
#include <pthread.h>


typedef struct file_info {
    const char *file_path;
    int is_done;
    char *err;
    long old_size_in_bytes;
    long new_size_in_bytes;

} file_info;


struct file_info **file_infos;
static int num_of_files = 0;

file_info* file_seen(const char *file_path){
    for(int i=0;i<num_of_files;i++){
        file_info *f = file_infos[i];
        if(f->file_path==file_path){
            return f;
        }
    }
    return NULL;
}

void *double_file_contents(void *fi_in){
    file_info *fi = (file_info*)fi_in;
    FILE *f = fopen(fi->file_path,"r+");
    if(f==NULL){
        fi->err= "Error opening file.";
        return NULL;
    }
    fseek(f, 0, SEEK_END);
    long original_byte_size = ftell(f);
    char *file_contents = malloc(sizeof(file_contents)*original_byte_size + sizeof(file_contents)*1);
    rewind(f);
    fread(file_contents, 1, original_byte_size, f);
    fwrite(file_contents , 1 , sizeof(file_contents) , f );
    fclose(f);
    free(file_contents);
    printf("Freed file contents from memory \n");
    file_contents = NULL;

    fi->old_size_in_bytes=original_byte_size;
    fi->new_size_in_bytes = original_byte_size*2;
    fi->is_done = 1;
    return NULL;
}

static PyObject *
double_file_contents_double_file_contents(PyObject *self, PyObject *args)
{
    const char *file_path = NULL;
    file_info *fi = NULL;
    if (!PyArg_ParseTuple(args, "s|:double_file_contents",&file_path)){
        return NULL;
    }

    if( access( file_path, F_OK ) == -1 ) {
        // file does not exist exists
        PyErr_SetString(PyExc_FileNotFoundError, "The file given does not exist");
        return NULL;
    }
    file_info *file_info_seen = file_seen(file_path);
    if(file_info_seen!=NULL){
        //file has been seen before
        if(!file_info_seen->is_done){
            PyErr_SetString(PyExc_RuntimeError, "The file given is already being processed");
            return NULL;
        }
        else {
            // the file has been seen and is also done. We will reuse the pointer to it.
            fi = file_info_seen;
            fi->is_done = 0;
            fi->new_size_in_bytes = 0;
            fi->old_size_in_bytes = 0;
            fi->err = 0;
        }
    }
    else{
        num_of_files++;
        file_infos =  realloc(file_infos,(sizeof file_infos)*num_of_files);
        fi = malloc(sizeof fi);
        fi->file_path=file_path;
        fi->is_done  = 0;
        file_infos[num_of_files-1] = fi;
    }

    pthread_t thrd;
    pthread_create(&thrd,NULL,&double_file_contents,(void *)fi);

    PyGILState_STATE gstate;
    gstate = PyGILState_Ensure();
    pthread_join(thrd,NULL);
    PyGILState_Release(gstate);

    Py_RETURN_NONE;
}


static PyObject *
double_file_contents_is_done(PyObject *self, PyObject *args){

    char *file_path = NULL;
    if (!PyArg_ParseTuple(args, "s:execute_runnable",&file_path)) {
        PyErr_SetString(PyExc_RuntimeError, "Unknown error");
        return NULL;
    }
    file_info *fi = file_seen(file_path);
    if (fi==NULL){
        PyErr_SetString(PyExc_ValueError, "No record of the path given.");
        return NULL;
    }
    printf("In is_done\n");
    if(fi->is_done){
        Py_RETURN_TRUE;
    }
    else{
        Py_RETURN_FALSE;
    }


}


static PyMethodDef DoubleFileContentsMethods[] = {
        {"double_file_contents",  double_file_contents_double_file_contents, METH_VARARGS, "Given a file path, reads the file and append the file contents in to the file"},
        {"is_done",  double_file_contents_is_done,   METH_VARARGS, "Checks whether the given file is done processing"},
        {NULL, NULL, 0, NULL}        /* Sentinel */
};

static struct PyModuleDef double_file_contentsmodule = {
        PyModuleDef_HEAD_INIT,
        "double_file_contents",   /* name of module */
        "This module double the file contents for a given file allows for multi-threaded file processing", /* module documentation, may be NULL */
        -1,       /* size of per-interpreter state of the module,
                 or -1 if the module keeps state in global variables. */
        DoubleFileContentsMethods
};

PyMODINIT_FUNC
PyInit_double_file_contents(void)
{
    return PyModule_Create(&double_file_contentsmodule);
}