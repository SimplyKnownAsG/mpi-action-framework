
#include "maf/maf.hpp"

#include <mpi.h>

class HelloWorldAction : public maf::Action, public maf::Transmittable<HelloWorldAction> {

public:

    HelloWorldAction() : maf::Action() {

    };

    void run() {
        maf::log("Hello World! (from c++)");
    };

};

int main(int argc, char* argv[]) {

    try {
        maf::ActionFactory::Register<HelloWorldAction>();

        std::vector<std::shared_ptr<maf::Action>> actions;
        actions.push_back(std::shared_ptr<maf::Action>(new HelloWorldAction));
        maf::BcastController controller(actions);
        controller.start();
    }
    catch (std::exception& ex) {
        maf::log("FAILED: ", ex.what());
        MPI_Abort(MPI_COMM_WORLD, -1);
    }
    catch (...) {
        maf::log("FAILED: no idea what happened");
        MPI_Abort(MPI_COMM_WORLD, -1);
    }
}
