from functools import wraps
import errno
import os
import signal
import time


class TimeoutError(Exception):
    pass


def timeout(seconds=10, error_message=os.strerror(errno.ETIME)):
    def decorator(func):
        def _handle_timeout(signum, frame):
            raise TimeoutError(error_message)

        def wrapper(*args, **kwargs):
            signal.signal(signal.SIGALRM, _handle_timeout)
            signal.alarm(seconds)
            try:
                result = func(*args, **kwargs)
            finally:
                signal.alarm(0)
            return result

        return wraps(func)(wrapper)

    return decorator


@timeout(1)
def func0():
    print "func0 finish"
    return


@timeout(1)
def func1():
    time.sleep(2)
    print "func1 finish"
    return


if __name__ == '__main__':
    try:
        func0()
        func1()
    except:
        print "timeout except"
