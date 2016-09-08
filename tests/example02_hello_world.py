'''test functionality in Python'''
import maf

from mpi4py.MPI import COMM_WORLD # this lets mpi4py handle MPI_Init and MPI_Finalize

import sys
import cPickle
import timeit

class HelloWorldAction(maf.Action):

    def __init__(self, my_string='Hello World!', my_int=0, my_float=0.0):
        maf.Action.__init__(self)
        self.my_string = my_string
        self.my_int = my_int
        self.my_float = my_float

    def run(self):
        maf.log('{} my_int: {} my_float: {}'.format(self.my_string, self.my_int, self.my_float))
    
    def serialize(self, archive):
        self.my_string = archive.rw_string(self.my_string)
        self.my_int = archive.rw_long_long(self.my_int)
        self.my_float = archive.rw_double(self.my_float)


class PickleSerialer(HelloWorldAction):

    def serialize(self, archive):
        data = dict(self.__dict__)
        if 'this' in data:
            del data['this'] # don't pickle the SWIG object
        string = cPickle.dumps(data)
        string = archive.rw_string(string)
        self.__dict__.update(cPickle.loads(string))


class TestBcastController(maf.BcastController):
    
    def __init__(self, action_class):
        maf.BcastController.__init__(self)
        self.action_class = action_class

    def main(self):
        act = self.action_class()
        self.distribute(act).run()
        act2 = self.action_class('Whatever', 2, 44)
        self.distribute(act2).run()
        act = self.action_class('The final act!', sys.maxint, sys.float_info.max)
        self.distribute(act).run()


maf.register(HelloWorldAction)
maf.register(PickleSerialer)

start = timeit.default_timer()
controller = TestBcastController(HelloWorldAction)
controller.start()
serializer_time = timeit.default_timer() - start

COMM_WORLD.Barrier()

start = timeit.default_timer()
controller = TestBcastController(PickleSerialer)
controller.start()
pickle_time = timeit.default_timer() - start

maf.log('SERIALIZE time: {}\n'
    '    PICKLE time   : {}'.format(serializer_time, pickle_time))
