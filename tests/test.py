'''test functionality in Python'''
from mpi4py.MPI import COMM_WORLD
import maf

class HelloWorldAction(maf.Action):
    def run(self):
        maf.maf_print("Hello World! (from Python)")

    def type_name(self):
        return "HelloWorldAction"
    
    def serialize(self):
        maf.maf_print('serializing {}'.format(self))


class TestBcastController(maf.BcastController):

    def main(self):
        pass
        # act = HelloWorldAction()
        # self.distribute(act);
        # act.run();


maf.register(HelloWorldAction)

controller = TestBcastController()
controller.start()
