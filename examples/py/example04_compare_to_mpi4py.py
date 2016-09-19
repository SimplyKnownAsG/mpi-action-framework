
from mpi4py.MPI import COMM_WORLD
import maf
import timeit
import numpy
import cPickle
import marshal

def bcast(obj, rank=0):
    if isinstance(obj, basestring):
        return maf.MafComm.World.bcast(obj)
    else:
        return marshal.loads(maf.MafComm.World.bcast(marshal.dumps(obj)))


maf.bcast = bcast

def compare(msg, mpi_func, maf_func, count=100):
    mpi_times = numpy.zeros(count)
    maf_times = numpy.zeros(count)
    for i in range(count):
        start = timeit.default_timer()
        mpi_func()
        mpi_times[i] = timeit.default_timer() - start
        start = timeit.default_timer()
        maf_func()
        maf_times[i] = timeit.default_timer() - start
    maf.log('{:<20} {:.4e} {:.4e} {:>6.2f}'.format(msg, mpi_times.mean(), maf_times.mean(), 
            100*((maf_times - mpi_times) / mpi_times).mean()))

compare('bcast(s)', lambda: COMM_WORLD.bcast('s'), lambda: maf.bcast('s'))
compare('bcast(1)', lambda: COMM_WORLD.bcast(1), lambda: maf.bcast(1))
compare('bcast(s*100)', lambda: COMM_WORLD.bcast('s'*100), lambda: maf.bcast('s'*100))
compare('bcast(s*1000)', lambda: COMM_WORLD.bcast('s'*1000), lambda: maf.bcast('s'*1000))

compare('bcast(range(1000))',
        lambda: COMM_WORLD.bcast(range(1000)),
        lambda: maf.bcast(range(1000)))

compare('bcast(range(10000))',
        lambda: COMM_WORLD.bcast(range(10000)),
        lambda: maf.bcast(range(10000)))

