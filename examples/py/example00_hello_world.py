'''Simple queue bcast controller'''
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
class HelloWorldAction(maf.Action):

    def run(self):
        maf.log('Hello World! (from Python) there are {} actions in the stack'.format(len(maf._ACTION_STACK)))

actions = [HelloWorldAction()]
controller = maf.BcastController(actions)
controller.start(AnnoyContext())
