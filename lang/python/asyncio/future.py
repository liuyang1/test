import asyncio


@asyncio.coroutine
def slow_operation(future, param):
    print("future %d run" % (param))
    yield from asyncio.sleep(1)
    print("future %d done" % (param))
    future.set_result((param, param ** 2))


def got_result(future):
    ret = future.result()
    print("future result " + str(ret))
    loop.stop()

loop = asyncio.get_event_loop()


def reg_future(param):
    future = asyncio.Future()
    # ensure_future not support at python 3.4.0 version
    # asyncio.ensure_future(slow_operation(future))
    asyncio.async(slow_operation(future, param))
    future.add_done_callback(got_result)

if __name__ == "__main__":
    reg_future(1)
    reg_future(2)
    try:
        loop.run_forever()
    finally:
        loop.close()
