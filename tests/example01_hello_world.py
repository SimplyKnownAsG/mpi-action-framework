'''test functionality in Python'''
from mpi4py.MPI import COMM_WORLD
import maf

@maf.action
class HelloWorldAction(maf.Action):

    def run(self):
        maf.log('Hello World! (from Python) there are {} actions in the stack'.format(len(maf._ACTION_STACK)))


class TestBcastController(maf.BcastController):

    def run(self):
        act = HelloWorldAction()
        self.bcast(act)
        act.run()


controller = TestBcastController()
controller.start()
