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


def test_success_expectations():
    callback.store_runnable(args, args=(1, 2))
    callback.execute_runnable()
    assert callback.get_result() == 100

    callback.store_runnable(noargs)
    callback.execute_runnable()
    assert callback.get_result() == 200

    callback.store_runnable(kwargs, kwargs=dict(kwarg1=1, kwarg2=2))
    callback.execute_runnable()
    assert callback.get_result() == 300

    callback.store_runnable(
        mixed_args_kwargs, args=(1, 2), kwargs=dict(kwarg1=1, kwarg2=2)
    )
    callback.execute_runnable()
    assert callback.get_result() == 400


def test_failure_expectations():

    e1 = None
    try:
        callback.store_runnable(noargs, 1)
    except Exception as e:
        e1 = e

    assert (
        type(e1) is TypeError
        and str(e1) == "store_runnable() takes at most 1 positional argument (2 given)"
    )

    e2 = None
    try:
        callback.store_runnable(noargs, blargs=("floop",))
    except Exception as e:
        e2 = e
    assert (
        type(e2) is TypeError
        and str(e2) == "'blargs' is an invalid keyword argument for store_runnable()"
    )

    e3 = None
    try:
        callback.store_runnable(noargs, args="I am not a tuple")
    except Exception as e:
        e3 = e
    assert type(e3) is TypeError and str(e3) == "args must be a tuple"

    e4 = None
    try:
        callback.store_runnable(noargs, kwargs="I am not a dict!")
    except Exception as e:
        e4 = e
    assert type(e4) is TypeError and str(e4) == "kwargs must be a dict"

    e5 = None
    try:
        callback.store_runnable(noargs, args=("There is no place for me :(",))
        callback.execute_runnable()
    except Exception as e:
        e5 = e
    assert (
        type(e5) is TypeError
        and str(e5) == "noargs() takes 0 positional arguments but 1 was given"
    )

    e6 = None
    try:
        callback.store_runnable(noargs, kwargs=dict(but_you_need_me="I am unwanted :("))
        callback.execute_runnable()
    except Exception as e:
        e6 = e
    assert (
        type(e6) is TypeError
        and str(e6) == "noargs() got an unexpected keyword argument 'but_you_need_me'"
    )


test_success_expectations()
test_failure_expectations()
