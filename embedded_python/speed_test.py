import subprocess
import time
from subprocess import DEVNULL

s1 = time.time()
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
total1 = time.time() - s1
s2 = time.time()
subprocess.run(["./run_py_files", "test1.py", "test2.py", "test3.py"], stdout=DEVNULL)
total2 = time.time() - s2

print(
    f"Running 3 python files in an embedded interpreter is {round(total1/total2,2)}% faster than running the python executable 3 times"
)
