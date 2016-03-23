import maf

class HelloWorld(maf.Action):

    def invoke(self):
        print 'hello world from rank {}!'.format(maf.RANK)

with maf.BroadcastController() as bc:
    bc.start_workers()

    hello = HelloWorld()
    hello.broadcast() # make the workers say hello

    hello.invoke() # say hello yourself :-)

