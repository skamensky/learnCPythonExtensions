#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>

static PyObject *
whoami_whoami(PyObject *self, PyObject *args)
{
    unsigned int cpu;
    unsigned int node;

    if (!PyArg_ParseTuple(args, ""))
        return NULL;


    // Get current CPU core and NUMA node via system call
    syscall(SYS_getcpu, &cpu, &node, NULL);
    return PyTuple_Pack(2,PyLong_FromLong(cpu),PyLong_FromLong(node));

};

static PyMethodDef WhoAmIMethods[] = {
        {"whoami",  whoami_whoami, METH_VARARGS, "Returns your identity."},
        {NULL, NULL, 0, NULL}        /* Sentinel */
};

static struct PyModuleDef whoamimodule = {
        PyModuleDef_HEAD_INIT,
        "whoami",   /* name of module */
        "This module tells you who your are", /* module documentation, may be NULL */
        -1,       /* size of per-interpreter state of the module,
                 or -1 if the module keeps state in global variables. */
        WhoAmIMethods
};

PyMODINIT_FUNC
PyInit_whoami(void)
{
    return PyModule_Create(&whoamimodule);
}