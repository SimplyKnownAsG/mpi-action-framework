#include "maf/maf.hpp"

#include <mpi.h>

class CounterContext : public maf::Context {
public:
    int count = 0;
};

MAF_ACTION(IncrementContextAction) {

    void run() override {
        this->context->as<CounterContext>()->count += 1;
    };
};

MAF_TEST_ACTION(test_bcast_action){
    void run() override{ auto action = std::shared_ptr<Action>(new IncrementContextAction);
auto controller = maf::BcastController();
auto context = std::shared_ptr<maf::Context>(new CounterContext);
this->assert_equal(0, context->as<CounterContext>()->count);
controller.context = context;
controller.bcast(action);
this->assert_equal(1, context->as<CounterContext>()->count);
controller.bcast(action);
this->assert_equal(2, context->as<CounterContext>()->count);
}
}
;

MAF_TEST_ACTION(test_bcast_action_vector){
    void run(){ auto action = std::shared_ptr<Action>(new IncrementContextAction);
std::vector<std::shared_ptr<Action>> actions;
auto controller = maf::BcastController();
auto context = std::shared_ptr<maf::Context>(new CounterContext);
this->assert_equal(0, context->as<CounterContext>()->count);
controller.context = context;

if (controller.rank == 0) {
    // empty list doesn't increment
    controller.bcast(actions);
    this->assert_equal(0, context->as<CounterContext>()->count);

    // increment by 3
    actions.insert(actions.begin(), 3, action);
    controller.bcast(actions);
    this->assert_equal(3, context->as<CounterContext>()->count);

    // increment by 4
    actions.push_back(action);
    controller.bcast(actions);
    controller.start(); // this terminates workers
} else {
    controller.start(context);
}

this->assert_equal(7, context->as<CounterContext>()->count);
}
}
;

MAF_TEST_MAIN()
