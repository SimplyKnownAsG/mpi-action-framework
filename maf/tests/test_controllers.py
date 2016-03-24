import unittest

import maf

class InvokeCounterAction(maf.Action):
    count = 0

    def invoke(self):
        InvokeCounterAction.count += 1


class ControllerTests(unittest.TestCase):

    def setUp(self):
        InvokeCounterAction.count = 0

    def help_fails_if_not_action(self, controller):
        if maf.RANK != 0:
            with self.assertRaises(AssertionError):
                controller.start_workers()
            raise maf.EndLoopAction()

    def test_wait_in_broadcast_fails_if_not_aciton(self):
        with maf.BroadcastController() as bc:
            self.help_fails_if_not_action(bc)
            maf.COMM_WORLD.bcast('hi')

    def test_wait_in_scatter_fails_if_not_aciton(self):
        with maf.ScatterController() as sc:
            self.help_fails_if_not_action(sc)
            maf.COMM_WORLD.scatter(['you gonna fail'] * maf.SIZE)

    def test_wait_in_receive_fails_if_not_aciton(self):
        with maf.ReceiveController() as rc:
            self.help_fails_if_not_action(rc)
            for worker_rank in range(1, maf.SIZE):
                maf.COMM_WORLD.send('you will fail!', dest=worker_rank)

    def test_async_controller_fails_if_notAction(self):
        with maf.AsyncController() as ac:
            self.help_fails_if_not_action(ac)
            for worker_rank in range(1, maf.SIZE):
                maf.COMM_WORLD.send('you will fail!', dest=worker_rank)

    def test_bcast_count_invocations(self):
        self.assertEqual(0, InvokeCounterAction.count)
        with maf.BroadcastController() as bc:
            bc.start_workers()

            act = InvokeCounterAction()
            act.broadcast().invoke()
            act.broadcast().invoke()

        self.assertEqual(2, InvokeCounterAction.count)

    def test_scatter_count_invocations(self):
        self.assertEqual(0, InvokeCounterAction.count)
        with maf.ScatterController() as sc:
            sc.start_workers()

            act = InvokeCounterAction()
            maf.COMM_WORLD.scatter([act] * maf.SIZE).invoke()
            maf.COMM_WORLD.scatter([act] * maf.SIZE).invoke()
            maf.COMM_WORLD.scatter([act] * maf.SIZE).invoke()

        self.assertEqual(3, InvokeCounterAction.count)

    @unittest.skip('does not seem to work, for some reason the action is not invoked until after the with loop')
    def test_async_count_invocations(self):
        self.assertEqual(0, InvokeCounterAction.count)
        with maf.AsyncController() as ac:
            # ac._mpi_wait_function = myrecv
            ac.start_workers()

            act = InvokeCounterAction2()
            print range(1,2)
            sent = 0
            for worker_rank in range(1, maf.SIZE):
                sent += 1
                maf.COMM_WORLD.Send(act, dest=worker_rank)
            self.assertEqual(1, sent)

        expectation = [0] + [1] * (maf.SIZE - 1)
        self.assertEqual(expectation[maf.RANK], InvokeCounterAction.count,
                'rank {}: {}[{}] != {}'.format(maf.RANK, expectation, maf.RANK, InvokeCounterAction.count))

    def test_start_workers_fails_without_controller(self):
        with self.assertRaises(Exception):
            maf.start_workers()

