'''
Running this test printed out the following:

Took test_no_threading_c 21 seconds
Took test_threading_c 5 seconds
Took test_no_threading_python 458 seconds
Took test_threading_python 514 seconds

C is faster than python, using C p threads is faster than C,
and using python with python threads is a the slowest (no surprises here)
'''


import sys
import os
from threading import Thread
sys.path.append(
    os.path.join(os.path.dirname(__file__), "./build", "lib.linux-x86_64-3.8"),
)
import find_primes
from time import time

def find_primes_python(upper_limit):
    non_primes = []
    primes = []
    for i in range(2,upper_limit):
        if i not in non_primes:
            primes.append(i)
            for j in range(i,upper_limit):
                if j%i==0:
                    non_primes.append(j)
    return primes

def test_no_threading_c(iterations):
    for _ in range(iterations):
        find_primes.find_primes(3000)


def test_threading_c(iterations):
    threads = [Thread(target=find_primes.find_primes,args=(3000,)) for i in range(iterations)]
    [t.start() for t in threads]
    [t.join() for t in threads]

def test_no_threading_python(iterations):
    for _ in range(iterations):
        find_primes_python(3000)

def test_threading_python(iterations):
    threads = [Thread(target=find_primes_python,args=(3000,)) for i in range(iterations)]
    [t.start() for t in threads]
    [t.join() for t in threads]


start = time()
test_no_threading_c(iterations=5000)
print(f'Took test_no_threading_c {int(time()-start)} seconds')

start = time()
test_threading_c(iterations=5000)
print(f'Took test_threading_c {int(time()-start)} seconds')


start = time()
test_no_threading_python(iterations=5000)
print(f'Took test_no_threading_python {int(time()-start)} seconds')

start = time()
test_threading_python(iterations=5000)
print(f'Took test_threading_python {int(time()-start)} seconds')
