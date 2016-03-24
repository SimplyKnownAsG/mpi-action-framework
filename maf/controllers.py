
import maf

__CONTROLLER = None


class _ActionController(object):

    def stop_workers(self):
        raise NotImplementedError()

    def start_workers(self):
        '''This is an infinite loop for workers; the only way to get out is to raise an exception!'''
        if maf.RANK == 0:
            return
        while True:
            action = self._mpi_wait_function()
            if not isinstance(action, maf.Action):
                raise AssertionError('Expected to receive a maf.Action, but got {}'.format(action))
            elif isinstance(action, maf.EndLoopAction):
                raise action
            action.invoke()

    def __enter__(self):
        global __CONTROLLER
        __CONTROLLER = self
        return self
    
    def __exit__(self, exc_type, exc_value, traceback):
        if maf.RANK == 0:
            self.stop_workers() # sends maf.EndLoopAction, that is raised by worker processes
        else:
            if isinstance(exc_value, maf.EndLoopAction):
                return True
        global __CONTROLLER
        __CONTROLLER = None


class BroadcastController(_ActionController):

    def _mpi_wait_function(self):
        return maf.COMM_WORLD.bcast(None)

    def stop_workers(self):
        maf.EndLoopAction().broadcast()


class ScatterController(_ActionController):

    def _mpi_wait_function(self):
        return maf.COMM_WORLD.scatter(None)

    def stop_workers(self):
        maf.COMM_WORLD.scatter([maf.EndLoopAction()] * maf.SIZE)


class ReceiveController(_ActionController):

    def _mpi_wait_function(self):
        act = maf.COMM_WORLD.sendrecv(source=0)
        return act

    def stop_workers(self):
        for worker_rank in range(1, maf.SIZE):
            maf.COMM_WORLD.send(maf.EndLoopAction(), dest=worker_rank)


class AsyncController(ReceiveController):

    pass


def start_workers():
    if __CONTROLLER is not None:
        __CONTROLLER.start_workers()
    else:
        raise Exception('Cannot start workers without an active controller')

def scatter_actions(actions):
    if __CONTROLLER is not None:
        __CONTROLLER.start_workers()
    else:
        raise Exception('Cannot start workers without an active controller')

