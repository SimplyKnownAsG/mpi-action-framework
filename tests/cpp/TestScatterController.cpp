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


int main(int argc, char* argv[]) {
    int exit_code = -1;

    try {
        auto factory = std::shared_ptr<maf::ActionFactory>(new maf::TActionFactory<IncrementContextAction>("IncrementContextAction"));
        maf::ActionFactory::Register(factory);

        // Create an action to use later
        auto increment_action = maf::ActionFactory::Create("IncrementContextAction");

        // run scatter controller with MPI_SIZE actions
        int size = maf::MafComm::World->size;
        std::vector<std::shared_ptr<maf::Action>> actions(size, increment_action);
        maf::ScatterController controller(actions);
        controller.start(std::shared_ptr<maf::Context>(new CounterContext));

        maf::barrier("=== 2 * MPI_SIZE");

        // run scatter controller with 2 * MPI_SIZE actions
        std::vector<std::shared_ptr<maf::Action>> actions2(2 * size, increment_action);
        maf::ScatterController controller2(actions2);
        controller2.start(std::shared_ptr<maf::Context>(new CounterContext));

        // run scatter contrller with MPI_SIZE + 1 actions
        actions.push_back(maf::ActionFactory::Create("IncrementContextAction"));
        maf::barrier("=== scattering ", actions.size(), " actions");
        maf::ScatterController uneven_controller(actions);

        try {
            uneven_controller.start(std::shared_ptr<maf::Context>(new CounterContext));
        }
        catch (maf::Exception* ex) {
        }

        exit_code = 0;
    }
    catch (std::exception* ex) {
        maf::log("FAILED: *", ex->what());
    }
    catch (...) {
        maf::log("FAILED: no idea what happened");
        MPI_Abort(MPI_COMM_WORLD, -1);
    }

    MPI_Finalize();
    return exit_code;
}
