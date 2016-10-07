#include "maf/maf.hpp"

#include <mpi.h>

class CounterContext : public maf::Context {
public:
    int count = 0;
};

class IncrementContextAction : public maf::Action {

public:

    void run() {
        this->context->as<CounterContext>()->count += 1;
    };

    std::string type_name() {
        return "IncrementContextAction";
    }

};

MAF_TEST_ACTION(test_bcast_action) {
    void run() {
        auto action = std::shared_ptr<Action>(new IncrementContextAction);
        auto controller = maf::BcastController();
        auto context = std::shared_ptr<maf::Context>(new CounterContext);
        this->assert_equal(0, context->as<CounterContext>()->count);
        controller.context = context;
        controller.bcast(action);
        this->assert_equal(1, context->as<CounterContext>()->count);
        controller.bcast(action);
        this->assert_equal(2, context->as<CounterContext>()->count);
    }
};

MAF_TEST_ACTION(test_bcast_action_vector) {
    void run() {
        auto action = std::shared_ptr<Action>(new IncrementContextAction);
        std::vector<std::shared_ptr<Action>> actions;
        auto controller = maf::BcastController();
        auto context = std::shared_ptr<maf::Context>(new CounterContext);
        this->assert_equal(0, context->as<CounterContext>()->count);
        controller.context = context;

        if (controller.rank == 0) {
            controller.bcast(actions);
            this->assert_equal(0, context->as<CounterContext>()->count);
            actions.insert(actions.begin(), 3, action);
            controller.bcast(actions);
            this->assert_equal(3, context->as<CounterContext>()->count);
            actions.push_back(action);
            controller.bcast(actions);
            controller.start(); // this terminates workers
        }
        else {
            controller.start(context);
        }

        this->assert_equal(7, context->as<CounterContext>()->count);
    }
};


int main(int argc, char* argv[]) {
    int exit_code = -1;

    auto factory = std::shared_ptr<maf::ActionFactory>((maf::ActionFactory*)(new maf::TActionFactory<IncrementContextAction>("IncrementContextAction")));
    maf::ActionFactory::Register(factory);

    try {
        auto controller = maf::TestController();
        controller.start();
        exit_code = 0;
    }
    catch (std::exception& ex) {
        maf::log("FAILED: &", ex.what());
    }
    catch (...) {
        maf::log("FAILED: no idea what happened");
    }

    if (exit_code == -1) {
        MPI_Abort(MPI_COMM_WORLD, -1);
    }

    MPI_Finalize();
    return exit_code;
}
