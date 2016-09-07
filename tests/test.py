'''test functionality in Python'''
from mpi4py.MPI import COMM_WORLD
import maf

class HelloWorldAction(maf.Action):

    def run(self):
        maf.maf_print("Hello World! (from Python)")

    def type_name(self):
        return "HelloWorldAction"
    
    def serialize(self, archive):
        maf.maf_print('serializing {}'.format(self))
    
    def __del__(self):
        maf.maf_print('HelloWorldAction.__del__')


class TestBcastController(maf.BcastController):

    def main(self):
        act = HelloWorldAction()
        self.distribute(act)
        act.run()


maf.register(HelloWorldAction)

controller = TestBcastController()
controller.start()
