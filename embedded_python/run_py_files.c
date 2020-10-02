#define PY_SSIZE_T_CLEAN
#include <Python.h>

//compile with
// gcc  -I/usr/include/python3.8/ -L/usr/lib/python3.8 run_py_files.c -lpython3.8  -o run_py_files
// run with
// ./run_py_files test1.py test2.py test3.py


int
main(int argc, char *argv[])
{
    wchar_t *program = Py_DecodeLocale(argv[0], NULL);
    if (program == NULL) {
        fprintf(stderr, "Fatal error: cannot decode argv[0]\n");
        exit(1);
    }
    Py_SetProgramName(program);  /* optional but recommended */
    Py_Initialize();


    for(int i=1;i<argc;++i){
        FILE *fp;
        printf("Running Python File \"%s\"\n",argv[i]);
        fp = fopen(argv[i], "r");
        PyRun_SimpleFile(fp,argv[i]);
        fclose(fp);
    }

    if (Py_FinalizeEx() < 0) {
        exit(120);
    }
    PyMem_RawFree(program);
    return 0;
}