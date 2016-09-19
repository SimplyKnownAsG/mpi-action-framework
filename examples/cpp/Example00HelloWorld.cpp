
#include "maf/maf.hpp"

#include <mpi.h>

class HelloWorldAction : public maf::Action {

public:

    HelloWorldAction() : maf::Action() {

    };

    void run() {
        maf::log("Hello World! (from c++)");
    };

    std::string type_name() {
        return "HelloWorldAction";
    }

};

int main(int argc, char* argv[]) {

    try {
        // std::shared_ptr<ActionFactory> factory((ActionFactory*)(new TActionFactory<HelloWorldAction>("HelloWorldAction")));
        auto factory = std::shared_ptr<maf::ActionFactory>((maf::ActionFactory*)(new maf::TActionFactory<HelloWorldAction>("HelloWorldAction")));
        maf::ActionFactory::Register(factory);

        std::vector<std::shared_ptr<maf::Action>> actions;
        actions.push_back(std::shared_ptr<maf::Action>(new HelloWorldAction));
        maf::BcastController controller(actions);
        controller.start();
    }
    catch (std::exception* ex) {
        maf::log("FAILED: ", ex->what());
        MPI_Abort(MPI_COMM_WORLD, -1);
    }
    catch (...) {
        maf::log("FAILED: no idea what happened");
        MPI_Abort(MPI_COMM_WORLD, -1);
    }

    MPI_Finalize();
}
