from distutils.core import setup, Extension

module1 = Extension("whoami", sources=["whoamimodule.c"])

setup(
    name="Spam",
    version="0.0.1",
    description="Runs stuff thru the C SYSTEM",
    ext_modules=[module1],
)
