import numpy
import reikna.cluda as cluda

N = 256
api = cluda.ocl_api()
thr = api.Thread.create()

program = thr.compile("""
KERNEL void multiply(
    GLOBAL_MEM float *dest,
    GLOBAL_MEM float *a,
    GLOBAL_MEM float *b)
{
    const SIZE_T i = get_local_id(0);
    dest[i] = a[i] * b[i];
}
""")

multiply = program.multiply

a = numpy.random.randn(N).astype(numpy.float32)
b = numpy.random.randn(N).astype(numpy.float32)
a_dev = thr.to_device(a)
b_dev = thr.to_device(b)
dest_dev = thr.empty_like(a_dev)

import time
t0 = time.time()
dest0 = a * b
t1 = time.time()
print str(t1 - t0)

t2 = time.time()
multiply(dest_dev, a_dev, b_dev, local_size=N, global_size=N)
t3 = time.time()
print str(t3 - t2)
dest = dest_dev.get()
