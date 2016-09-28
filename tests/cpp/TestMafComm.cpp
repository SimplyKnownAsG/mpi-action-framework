#include "maf/maf.hpp"

MAF_TEST_ACTION(test_bcast_string) {
    void run() {
        auto world = maf::MafComm::World;
        std::string str = std::string((world->rank + 3), 'm');

        if (world->rank == 0) {
            this->assert_equal("mmm", str);
        }
        else {
            this->assert_not_equal("mmm", str);
        }

        str = world->bcast(str);

        this->assert_equal("mmm", str);
    }
};

MAF_TEST_ACTION(test_int) {
    void run() {
        auto world = maf::MafComm::World;
        int val = (world->rank + 3) * 3;

        if (world->rank == 0) {
            this->assert_equal(9, val);
        }
        else {
            this->assert_not_equal(9, val);
        }

        val = world->bcast(val);

        this->assert_equal(9, val);
    }
};


MAF_TEST_ACTION(test_float) {
    void run() {
        auto world = maf::MafComm::World;
        float val = (world->rank + 3.3) * 3;

        if (world->rank == 0) {
            this->assert_close(9.9f, val, 1e-5f);
        }
        else {
            this->assert_not_close(9.9f, val, 1e-2f);
        }

        val = world->bcast(val);

        this->assert_close(9.9f, val, 1e-5f);
    }
};


MAF_TEST_ACTION(test_double) {
    void run() {
        auto world = maf::MafComm::World;
        double val = (world->rank + 3.3) * 3;

        if (world->rank == 0) {
            this->assert_close(9.9, val, 1e-5);
        }
        else {
            this->assert_not_close(9.9, val, 1e-2);
        }

        val = world->bcast(val);

        this->assert_close(9.9, val, 1e-5);
    }
};

int main(int argc, char* argv[]) {
    int exit_code = -1;

    try {
        std::vector<std::shared_ptr<maf::Action>> actions {
            std::shared_ptr<maf::Action>(new test_bcast_string),
            std::shared_ptr<maf::Action>(new test_int),
            std::shared_ptr<maf::Action>(new test_float),
            std::shared_ptr<maf::Action>(new test_double),
        };
        auto controller = maf::BcastController(actions);
        controller.start();
        exit_code = 0;
    }
    catch (std::exception* ex) {
        maf::log("FAILED: *", ex->what());
    }
    catch (...) {
        maf::log("FAILED: no idea what happened");
    }

    MPI_Finalize();
    return exit_code;
}
