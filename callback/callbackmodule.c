#define PY_SSIZE_T_CLEAN
#include <Python.h>

static PyObject *callback_store  = NULL;
static PyObject *args_store      = NULL;
static PyObject *kwargs_store    = NULL;
static PyObject *callback_result = NULL;

static PyObject *
callback_storerunnable(PyObject *self, PyObject *args,PyObject *kwargs)
{
    PyObject *temp_callable = NULL;
    PyObject *temp_args = NULL;
    PyObject *temp_kwargs = NULL;

    static char* keywords[] = {"","args","kwargs" ,NULL};

    if (!PyArg_ParseTupleAndKeywords(args,kwargs, "O|$OO:store_runnable",keywords,&temp_callable, &temp_args,&temp_kwargs)){
        return NULL;
    }
    if (!PyCallable_Check(temp_callable)) {
        PyErr_SetString(PyExc_TypeError, "First parameter must be callable");
        return NULL;
    }
    if (temp_args!=NULL && !PyTuple_Check(temp_args)) {
        PyErr_SetString(PyExc_TypeError, "args must be a tuple");
        return NULL;
    }
    if (temp_kwargs!=NULL && !PyDict_Check(temp_kwargs)) {
        PyErr_SetString(PyExc_TypeError, "kwargs must be a dict");
        return NULL;
    }


    // args must not be NULL, use an empty tuple if no arguments are needed.
    // If no named arguments are needed, kwargs can be NULL.
    if(temp_args==NULL){
        temp_args=PyTuple_New(0);
    }

    Py_XINCREF(temp_callable);
    Py_XINCREF(temp_args);
    Py_XINCREF(temp_kwargs);

    Py_XDECREF(callback_store);
    Py_XDECREF(args_store);
    Py_XDECREF(kwargs_store);

    callback_store = temp_callable;
    args_store = temp_args;
    kwargs_store = temp_kwargs;

    Py_RETURN_NONE;
};

static PyObject *
callback_executerunnable(PyObject *self, PyObject *args)
{

    PyObject *result;
    if (!PyArg_ParseTuple(args, ":execute_runnable")) {
            PyErr_SetString(PyExc_TypeError, "Unknown error");
            return NULL;
    }

    else{


        result = PyObject_Call(callback_store, args_store,kwargs_store);

        if (result == NULL){
            return NULL; /* Pass error back */
        }

        //if we had a result stored earlier, clear it up before overwriting
        Py_XDECREF(callback_result);
        callback_result = result;

        Py_RETURN_NONE;
    }

};

static PyObject *
callback_getresult(PyObject *self, PyObject *args){
    if (!PyArg_ParseTuple(args, ":execute_runnable")) {
        PyErr_SetString(PyExc_TypeError, "Unknown error");
        return NULL;
    }
    else{
        if(callback_result==NULL){
            PyErr_SetString(PyExc_ValueError, "Return value is not set");
            return NULL;
        }
        else{
            return callback_result;
        }
    }

}


static PyMethodDef CallbackMethods[] = {
        {"store_runnable",  (PyCFunction)(void(*)(void))callback_storerunnable,   METH_VARARGS | METH_KEYWORDS, "Stores the given python function for later use."},
        {"execute_runnable",  callback_executerunnable, METH_VARARGS, "Executes function given at an earlier point."},
        {"get_result",  callback_getresult, METH_VARARGS, "Returns the result of the callback function."},
        {NULL, NULL, 0, NULL}        /* Sentinel */
};

static struct PyModuleDef callbackmodule = {
        PyModuleDef_HEAD_INIT,
        "callback",   /* name of module */
        "This module stores a function in the CPython runtime for later user and allows you to run it later", /* module documentation, may be NULL */
        -1,       /* size of per-interpreter state of the module,
                 or -1 if the module keeps state in global variables. */
        CallbackMethods
};

PyMODINIT_FUNC
PyInit_callback(void)
{
    return PyModule_Create(&callbackmodule);
}