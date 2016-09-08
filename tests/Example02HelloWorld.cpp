#include "maf/maf.hpp"

#include <mpi.h>

class HelloWorldAction : public maf::Action {

public:

    std::string my_string;
    int my_int;
    float my_float;
    double my_double;

    HelloWorldAction() : maf::Action() {

    };

    HelloWorldAction(std::string string_, int int_, float float_, double double_) : maf::Action() {
        this->my_string = string_;
        this->my_int = int_;
        this->my_float = float_;
        this->my_double = double_;
    };

    void run() {
        maf::mpi_print("Hello World! (from c++)",
            "\n     my_string: `", this->my_string,
            "`\n     my_int: `", this->my_int,
            "`\n     my_float: `", this->my_float,
            "`\n     my_double: `", this->my_double, "`"
            );
    };

    void serialize(std::shared_ptr<maf::Archive> archive) {
        (*archive) & this->my_string;
        (*archive) & this->my_int;
        (*archive) & this->my_float;
        (*archive) & this->my_double;
    };

    std::string type_name() {
        return "HelloWorldAction";
    }

};

class TestBcastController : public maf::BcastController {
public:
    void main() {
        std::shared_ptr<maf::Action> act = std::shared_ptr<maf::Action>(new HelloWorldAction);
        this->distribute(act);
        act->run();
        act = std::shared_ptr<maf::Action>(new HelloWorldAction("string 1", 2, 3.3, 4.4));
        this->distribute(act);
        act->run();
        act = std::shared_ptr<maf::Action>(new HelloWorldAction("string 2", 4, 66.66, 88.88e88));
        this->distribute(act);
        act->run();
    };
};

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    try {
        // std::shared_ptr<ActionFactory> factory((ActionFactory*)(new TActionFactory<HelloWorldAction>("HelloWorldAction")));
        auto factory = std::shared_ptr<maf::ActionFactory>((maf::ActionFactory*)(new maf::TActionFactory<HelloWorldAction>("HelloWorldAction")));
        maf::ActionFactory::Register(factory);

        TestBcastController controller;
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
