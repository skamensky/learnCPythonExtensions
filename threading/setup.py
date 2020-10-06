from distutils.core import setup, Extension

module1 = Extension("double_file_contents", sources=["threads_with_python.c"])

setup(
    name="Double File Contents",
    version="0.0.1",
    description="Uses C pthreads to double file contents",
    ext_modules=[module1],
)
