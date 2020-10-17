from distutils.core import setup, Extension

module1 = Extension("find_primes", sources=["threads_with_python.c"])

setup(
    name="Finds Prime Numbers",
    version="0.0.1",
    description="Uses C pthreads to find prime numbers",
    ext_modules=[module1],
)
