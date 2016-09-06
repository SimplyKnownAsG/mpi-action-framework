'''test functionality in Python'''
from mpi4py.MPI import COMM_WORLD
import maf
from maf import mpi_print
import unittest

rank = COMM_WORLD.Get_rank()

things = [maf.Thing('Thing {} from {}'.format(ii, rank)) for ii in range(10)]

mpi_print('bcasting thing[0]')
t0 = COMM_WORLD.bcast(things[0])
mpi_print('done with that')
if rank != 0:
    mpi_print('{}, {}'.format(t0.index, things[0].index))

class Item(maf.Thing):

    def __init__(self, name):
        self.double = 14.1 * rank
        self.long = 12
        maf.Thing.__init__(self, name)

    def serialize(self, serializer):
        self.double = serializer.serialize_double(self.double)
        self.long = serializer.serialize_int(self.long)
        mpi_print('hello from {}'.format(self.name))

i = Item('Item from {}'.format(rank))
mpi_print('i.double={}, i.long={}'.format(i.double, i.long))
i.bcast()
mpi_print('i.double={}, i.long={}'.format(i.double, i.long))

if i.double == 0.0:
    mpi_print('Success!!')
else:
    mpi_print('FAILED')

assert 'EndLoopAction' in maf.Action.RegisteredNames(), 'EndLoopAction is not registered!!'

class HelloWorldAction(maf.Action):

    def run(self):
        mpi_print('Hello From Python!')

maf.register(HelloWorldAction)

# maf.Action.Register('WhatAction', WhatAction.__init__)
mpi_print('FAILED: still need to figure out callback to create action')
