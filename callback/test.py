import sys
import os

sys.path.append(
    os.path.join(os.path.dirname(__file__), "./build", "lib.linux-x86_64-3.8"),
)
import callback


def args(arg1, arg2):
    return 100


def noargs():
    return 200


def kwargs(*, kwarg1, kwarg2):
    return 300


def mixed_args_kwargs(arg1, arg2, *, kwarg1, kwarg2):
    return 400


callback.store_runnable(args, args=(1, 2))
callback.execute_runnable()
print(callback.get_result())


callback.store_runnable(noargs)
callback.execute_runnable()
print(callback.get_result())

callback.store_runnable(kwargs, kwargs=dict(kwarg1=1, kwarg2=2))
callback.execute_runnable()
print(callback.get_result())

callback.store_runnable(mixed_args_kwargs, args=(1, 2), kwargs=dict(kwarg1=1, kwarg2=2))
callback.execute_runnable()
print(callback.get_result())
