import sys
import os

current_dir = os.path.dirname(__file__)
sys.path.append(
    os.path.join(current_dir, "./build", "lib.linux-x86_64-3.8"),
)
import double_file_contents
file_p1 = "/tmp/test.txt"
double_file_contents.double_file_contents(file_p1)
print(double_file_contents.is_done(file_p1))
import time
time.sleep(1)
