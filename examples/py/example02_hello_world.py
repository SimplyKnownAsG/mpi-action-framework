'''test functionality in Python'''
import maf

import sys
import cPickle
import marshal
import timeit

@maf.action
class MafSerializer(maf.Action):

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


@maf.action
class PickleSerialer(MafSerializer):

    def serialize(self, archive):
        data = dict(self.__dict__)
        if 'this' in data:
            del data['this'] # don't pickle the SWIG object
        string = cPickle.dumps(data)
        string = archive.rw_string(string)
        self.__dict__.update(cPickle.loads(string))


@maf.action
class MarshalSerialer(MafSerializer):

    def serialize(self, archive):
        data = dict(self.__dict__)
        if 'this' in data:
            del data['this'] # don't pickle the SWIG object
        string = marshal.dumps(data)
        string = archive.rw_string(string)
        self.__dict__.update(marshal.loads(string))


class TestBcastController(maf.BcastController):
    
    def __init__(self, action_class):
        maf.BcastController.__init__(self)
        self.action_class = action_class

    def run(self):
        act = self.action_class()
        self.bcast(act)
        act2 = self.action_class('Whatever', 2, 44)
        self.bcast(act2)
        act = self.action_class('The final act!', sys.maxint, sys.float_info.max)
        self.bcast(act)

maf.barrier('==== Starting tests with MafSerializer')

start = timeit.default_timer()
controller = TestBcastController(MafSerializer)
controller.start()
serializer_time = timeit.default_timer() - start

maf.barrier('==== Starting tests with PickleSerializer')

start = timeit.default_timer()
controller = TestBcastController(PickleSerialer)
controller.start()
pickle_time = timeit.default_timer() - start

maf.barrier('==== Starting tests with MarshalSerializer')

start = timeit.default_timer()
controller = TestBcastController(MarshalSerialer)
controller.start()
marshal_time = timeit.default_timer() - start

maf.barrier('==== Done with tests')

maf.log(              'SERIALIZE time: {}\n'
    '                  PICKLE time   : {}\n'
    '                  MARSHAL time  : {}'
    .format(serializer_time, pickle_time, marshal_time))
