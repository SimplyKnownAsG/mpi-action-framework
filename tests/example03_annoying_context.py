'''test functionality in Python'''
from mpi4py.MPI import COMM_WORLD
import maf


class AnnoyContext(maf.Context):

    def __init__(self):
        maf.Context.__init__(self)
        self.annoyance_count = 0

    def annoy(self):
        self.annoyance_count += 1
        if self.annoyance_count > 2:
            maf.log("Ugh, so annoyed!!! {}".format(self.annoyance_count))
        else:
            maf.log("Nope, not annoyed... {}".format(self.annoyance_count))


@maf.action
class AnnoyAction(maf.Action):

    def run(self):
        maf.log("Are you annoyed yet?")
        self.context.ptr().annoy()


context = AnnoyContext()
actions = [AnnoyAction()] * 3

controller = maf.BcastController(actions)
controller.start(context)
