import subprocess
import time
from subprocess import DEVNULL
import timeit

def separate_processes():
    subprocess.run(
        [
            "python3.8",
            "test1.py",
            "&&",
            "python3.8",
            "test2.py",
            "&&",
            "python3.8",
            "test3.py",
        ],
        stdout=DEVNULL,
    )

def single_embedded_python_process():
    subprocess.run(["./run_py_files", "test1.py", "test2.py", "test3.py"], stdout=DEVNULL)

multi =  timeit.timeit(separate_processes, number=100)
single = timeit.timeit(single_embedded_python_process, number=100)

print(
    f"Running 3 python files in an embedded interpreter is {round(multi/single,2)}% faster than running the python executable 3 times"
)
