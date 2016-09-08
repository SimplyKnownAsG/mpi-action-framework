'''test functionality in Python'''
from mpi4py.MPI import COMM_WORLD
import maf

class HelloWorldAction(maf.Action):

    def run(self):
        maf.maf_print('Hello World! (from Python) there are {} actions in the stack'.format(len(maf._ACTION_STACK)))

    def type_name(self):
        return "HelloWorldAction"


class TestBcastController(maf.BcastController):

    def main(self):
        act = HelloWorldAction()
        self.distribute(act)
        act.run()


maf.register(HelloWorldAction)

controller = TestBcastController()
controller.start()
