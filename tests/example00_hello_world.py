'''Simple queue bcast controller'''
from mpi4py.MPI import COMM_WORLD
import maf

class HelloWorldAction(maf.Action):

    def run(self):
        maf.log('Hello World! (from Python) there are {} actions in the stack'.format(len(maf._ACTION_STACK)))

maf.register(HelloWorldAction)

actions = [HelloWorldAction()]
controller = maf.BcastController(actions)
controller.start()
