#include "maf/maf.hpp"

MAF_TEST_ACTION(test_bcast_string) {
    void run() {
        auto world = maf::MafComm::World;
        std::string str = std::string((world->rank + 3), 'm');

        if (world->rank == 0) {
            this->assert_equal(std::string("mmm"), str);
        }
        else {
            this->assert_not_equal(std::string("mmm"), str);
        }

        str = world->bcast(str);

        this->assert_equal(std::string("mmm"), str);
    }
};

MAF_TEST_ACTION(test_bcast_int) {
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

MAF_TEST_ACTION(test_bcast_int_vector) {
    void run() {
        auto world = maf::MafComm::World;
        std::vector<int> val(world->size - world->rank, (world->rank + 3) * 3);
        std::vector<int> expected {9, 9, 9};

        if (world->rank == 0) {
            this->assert_equal(expected, val);
        }
        else {
            this->assert_not_equal(expected, val);
        }

        val = world->bcast(val);
        this->assert_equal(expected, val);
    }
};

MAF_TEST_ACTION(test_bcast_float) {
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

MAF_TEST_ACTION(test_bcast_float_vector) {
    void run() {
        auto world = maf::MafComm::World;
        std::vector<float> val(world->size - world->rank, (world->rank + 3) * 3.3);
        std::vector<float> expected {9.9f, 9.9f, 9.9f};

        if (world->rank == 0) {
            this->assert_close(expected, val, 1e-5f);
        }
        else {
            this->assert_not_equal(expected, val);
        }

        val = world->bcast(val);
        this->assert_close(expected, val, 1e-5f);
    }
};

MAF_TEST_ACTION(test_bcast_double) {
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

MAF_TEST_ACTION(test_bcast_double_vector) {
    void run() {
        auto world = maf::MafComm::World;
        std::vector<double> val(world->size - world->rank, (world->rank + 3) * 3.3);
        std::vector<double> expected {9.9, 9.9, 9.9};

        if (world->rank == 0) {
            this->assert_close(expected, val, 1e-5);
        }
        else {
            this->assert_not_equal(expected, val);
        }

        val = world->bcast(val);
        this->assert_close(expected, val, 1e-5);
    }
};

MAF_TEST_MAIN()
