'''
This module provides an abstract class to be used to implement "MPI actions."

MPI actions are tasks, activities, or work that can be executed on the worker nodes. The standard
workflow is essentially that the master node creates an :py:class:`~maf.actions.Action`,
sends it to the workers, and then both the master and the wokers
:py:meth:`invoke() <maf.actions.Action.invoke>` together. For example:

.. list-table:: Sample MPI Action Workflow
   :widths: 5 60 35
   :header-rows: 1

   * - Step
     - Code
     - Notes
   * - 1
     - **master**: :py:class:`someAction = SomeAction() <maf.actions.Action>`
     
       **worker**: :code:`action = maf.MPI_COMM.bcast(None, root=0)`
     - **master**: Initializing a distribute state action.
     
       **worker**: Waiting for something to do, as determined by the master, this happens within the 
       worker's :py:meth:`~maf.operators.MpiOperator.workerOperate`.
   * - 2
     - **master**: :code:`maf.MPI_COMM.bcast(someAction, root=0)`
     
       **worker**: :code:`action = maf.MPI_COMM.bcast(None, root=0)`
     - **master**: Broadcasts a distribute state action to all the worker nodes
     
       **worker**: Receives the action from the master, which is a
       :py:class:`~maf.actions.py.SomeAction`.
   * - 3
     - **master**: :code:`someAction.invoke()`
     
       **worker**: :code:`action.invoke()`
     - Both invoke the action, and are in sync. Any broadcast or receive within the action should
       also be synced up.

In order to create a new, custom MPI Action, inherit from :py:class:`~maf.actions.Action`,
and override the :py:meth:`~maf.actions.Action.invokeHook` method.

'''
import gc
import pickle
import cPickle
import time

import maf

class Action(object):
    '''
    Base of all MPI actions.
    
    MPI Actions are tasks that can be executed without needing lots of other
    information. When a worker node sits in it's main loop, and receives an MPI
    Action, it will simply call :py:method:`~maf.actions.Action.invoke`.
    '''
    
    def __init__(self):
        self.transmitted = False

    def _exec_mpi_function(self, obj, mpi_function):
        if obj is None:
            obj = self
        try:
            return mpi_function(obj, root=0)
        except (pickle.PicklingError, cPickle.PickleError) as error:
            runLog.error('Failed to {} {}.'.format(mpi_function.__name__, obj))
            runLog.error(error)
            raise

    def broadcast(self, obj=None):
        '''A wrapper around ``bcast``, on the master node can be run with an equals sign, so that it
        can be consistent within both master and worker nodes.

        Parameters
        ----------
        obj :
            This is any object that can be broadcast, if it is None, then it will broadcast itself.
        
        Notes
        -----
        The standard ``bcast`` method creates a new instance even for the root process. Consequently,
        when passing an object, references can be broken to the original object. Therefore, this
        method, returns the original object when called by the master node, or the broadcasted
        object when called on the worker nodes.
        '''
        if maf.SIZE > 1:
            result = self._exec_mpi_function(obj, maf.COMM_WORLD.bcast)
        # the following if-branch prevents the creation of duplicate objects on the master node
        # if the object is large with lots of links, it is prudent to call gc.collect()
        if obj is None and maf.RANK == 0:
            return self
        else:
            return result

    def gather(self, obj=None):
        '''A wrapper around ``MPI_COMM.gather``.

        Parameters
        ----------
        obj :
            This is any object that can be gathered, if it is None, then it will gather itself.
        
        Notes
        -----
        The returned list will contain a reference to the original gathered object, without making a copy of it.
        '''
        if maf.SIZE > 1:
            result = self._exec_mpi_function(obj, maf.COMM_WORLD.gather)
            if maf.RANK == 0:
                # this cannot be result[0] = obj or self, because 0.0, 0, [] all eval to False
                if obj is None:
                    result[0] = self
                else:
                    result[0] = obj
        else:
            result = [obj]
        return result

    def supply_state(self, *args, **kwargs):
        pass

    def invoke(self):
        '''
        This method is called by worker nodes, and passed the worker node's operator, reactor and
        settings file.
        '''
        raise NotImplementedError('Must subclass maf.Action.invoke before it can be used!')


class EndLoopAction(Action, Exception):

    pass

