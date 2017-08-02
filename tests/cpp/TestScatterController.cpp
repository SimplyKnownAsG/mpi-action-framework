#include "maf/maf.hpp"

#include <mpi.h>

class CounterContext : public maf::Context {
public:
    int count = 0;
};

MAF_ACTION(IncrementContextAction) {
    this->context->as<CounterContext>()->count += 1;
}

MAF_TEST_ACTION(test_equal_mpi_size) {
    // run scatter controller with MPI_SIZE actions
    int size = maf::MafComm::World->size;
    std::vector<std::shared_ptr<maf::Action>> actions(
      size, maf::ActionFactory::Create<IncrementContextAction>());
    maf::ScatterController controller(actions);
    auto context = std::shared_ptr<maf::Context>(new CounterContext);
    controller.start(context);
    this->assert_equal(1, context->as<CounterContext>()->count);
}

MAF_TEST_ACTION(test_twice_mpi_size) {
    // run scatter controller with 2 * MPI_SIZE actions
    int size = maf::MafComm::World->size;
    std::vector<std::shared_ptr<maf::Action>> actions(
      2 * size, maf::ActionFactory::Create<IncrementContextAction>());
    maf::ScatterController controller(actions);
    controller.start(std::shared_ptr<maf::Context>(new CounterContext));
}

MAF_TEST_MAIN()
