from mpi4py.MPI import COMM_WORLD 
import maf

@maf.test
def test_bcast_string(self):
    '''here is some doc for you'''
    maf.log('here.')
    world = maf.MafComm.World
    maf.log('here..')
    val = 'm' * (world.rank + 3)

    maf.log('here...')
    if world.rank == 0:
        self.assert_equal("mmm", val)
    else:
        self.assert_not_equal("mmm", val)
    maf.log('here....')

    val = world.bcast(val)
    maf.log('here.....')
    self.assert_equal("mmm", val)


@maf.test
def test_bcast_int(self):
    world = maf.MafComm.World
    val = (world.rank + 3) * 3

    if (world.rank == 0):
        self.assert_equal(9, val)
    else:
        self.assert_not_equal(9, val)

    val = world.bcast(val)
    self.assert_equal(9, val)


@maf.test
def test_bcast_float(self):
    world = maf.MafComm.World
    val = (world.rank + 3.3) * 3

    if (world.rank == 0):
        self.assert_close(9.9, val, 1e-5)
    else:
        self.assert_not_close(9.9, val, 1e-2)

    val = world.bcast(val)
    self.assert_close(9.9, val, 1e-5)


@maf.test
def test_bcast_double(self):
    world = maf.MafComm.World
    val = (world.rank + 3.3) * 3

    if (world.rank == 0):
        self.assert_close(9.9, val, 1e-5)
    else:
        self.assert_not_close(9.9, val, 1e-2)
    

    val = world.bcast(val)
    self.assert_close(9.9, val, 1e-5)


if __name__ == '__main__':
    maf.barrier('ERROR: these tests fail due to an error in SWiG. See https://github.com/swig/swig/issues/773')
    # controller = maf.TestController()
    # controller.start()
