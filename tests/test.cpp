
#include "maf/maf.hpp"

#include <mpi.h>

class HelloWorld : public maf::Action {

public:
    void run() {
        maf::mpi_print("Hello World!");
    };

    std::string type_name() {
        return "HelloWorld";
    }

};

class TestBcastController : public maf::BcastController {
public:
    void main() {
        maf::Action* act = new HelloWorld;
        this->distribute(act);
        act->run();
        delete act;
    };
};

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    maf::Action::Register("HelloWorld", &maf::create_action<HelloWorld>);

    TestBcastController controller;
    controller.start();

    MPI_Finalize();
}
