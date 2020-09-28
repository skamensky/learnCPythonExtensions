from distutils.core import setup, Extension

module1 = Extension("callback", sources=["callbackmodule.c"])

setup(
    name="Callback",
    version="0.0.1",
    description="POC of Python Functions Running in the CPython Runtime",
    ext_modules=[module1],
)
