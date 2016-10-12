
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

class TestBcastController : public maf::BcastController {
public:
    void run() {
        this->bcast(std::shared_ptr<maf::Action>(new HelloWorldAction));
    };
};

int main(int argc, char* argv[]) {
    try {
        maf::ActionFactory::Register<HelloWorldAction>();

        TestBcastController controller;
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
