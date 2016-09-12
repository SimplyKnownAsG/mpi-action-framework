'''test functionality in Python'''
from mpi4py.MPI import COMM_WORLD
import maf

class HelloWorldAction(maf.Action):

    def run(self):
        maf.log('Hello World! (from Python) there are {} actions in the stack'.format(len(maf._ACTION_STACK)))


class TestBcastController(maf.BcastController):

    def main(self):
        act = HelloWorldAction()
        self.bcast(act)
        act.run()


maf.register(HelloWorldAction)

controller = TestBcastController()
controller.start()
