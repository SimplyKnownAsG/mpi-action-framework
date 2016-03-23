
import unittest
from mpi4py.MPI import COMM_WORLD

import maf


class ActionTests(unittest.TestCase):
    
    @classmethod
    def setUpClass(cls):
        assert maf.SIZE > 3, 'must run with more than 3 processors'

    def test_is_abstract(self):
        act = maf.Action()
        with self.assertRaises(NotImplementedError):
            act.invoke()

    def test_broadcast_self(self):
        act = maf.Action()
        act.data = maf.RANK
        if maf.RANK == 0:
            act2 = act.broadcast()
            self.assertEqual(id(act), id(act2)) # avoid duplication
        else:
            act2 = COMM_WORLD.bcast(None, root=0)
            self.assertNotEqual(act.data, act2.data)
        self.assertIsInstance(act2, maf.Action)
        self.assertEqual(act2.data, 0)

    def test_broadcast_object(self):
        act = maf.Action()
        if maf.RANK == 0:
            data = act.broadcast('turtle')
        else:
            data = COMM_WORLD.bcast(None, root=0)
        self.assertEqual('turtle', data)

    def test_gather_self(self):
        act = maf.Action()
        act.data = [maf.RANK] * maf.RANK
        if maf.RANK == 0:
            acts = act.gather()
            self.assertEqual(maf.SIZE, len(acts))
            for rank in range(maf.SIZE):
                self.assertEqual(rank, len(acts[rank].data))
                for ii in range(rank):
                    self.assertEqual(rank, acts[rank].data[ii])
        else:
            self.assertEqual(None, COMM_WORLD.gather(act))

    def test_gather_object(self):
        act = maf.Action()
        if maf.RANK == 0:
            data = act.gather([maf.RANK] * maf.RANK)
            self.assertEqual(maf.SIZE, len(data))
            for rank in range(maf.SIZE):
                self.assertEqual(rank, len(data[rank]))
                for ii in range(rank):
                    self.assertEqual(rank, data[rank][ii])
        else:
            self.assertEqual(None, COMM_WORLD.gather([maf.RANK] * maf.RANK))

    @unittest.skip('need to develop feature...')
    def test_scatter(self):
        raise NotImplementedError

    @unittest.skip('need to develop feature...')
    def test_supply_state(self):
        act = maf.Action()

