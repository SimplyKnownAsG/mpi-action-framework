
#include "maf/maf.hpp"

#include <mpi.h>

class HelloWorldAction : public maf::Action {

public:

    HelloWorldAction() : maf::Action() {

    };

    void run() {
        maf::mpi_print("Hello World! (from c++)");
    };

    std::string type_name() {
        return "HelloWorldAction";
    }

};

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

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
        maf::mpi_print("FAILED: ", ex->what());
    }
    catch (...) {
        maf::mpi_print("FAILED: no idea what happened");
    }

    MPI_Finalize();
}
