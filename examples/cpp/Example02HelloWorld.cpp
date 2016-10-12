#include "maf/maf.hpp"

#include <mpi.h>

class HelloWorldAction : public maf::Action, public maf::Transmittable<HelloWorldAction> {

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

    void run() override {
        maf::log("Hello World! (from c++)",
                 "     my_string: `", this->my_string,
                 "`     my_int: `", this->my_int,
                 "`     my_float: `", this->my_float,
                 "`     my_double: `", this->my_double, "`"
                );
    };

    void serialize(std::shared_ptr<maf::Archive> archive) override {
        (*archive) & this->my_string;
        (*archive) & this->my_int;
        (*archive) & this->my_float;
        (*archive) & this->my_double;
    };

};


static std::vector<std::shared_ptr<maf::Action>> GetActions(int action_count) {
    std::vector<std::shared_ptr<maf::Action>> actions;

    for (int ii = 0; ii < action_count; ii++) {
        actions.push_back(std::shared_ptr<maf::Action>(new HelloWorldAction(std::string(ii, 's'), 2 * ii, 3.3 * ii, 4.4 * ii * ii)));
    }

    return actions;
}

class TestBcastController : public maf::BcastController {
public:

    void run() override {
        for (auto action : GetActions(3)) {
            this->bcast(action);
        }

        this->bcast(GetActions(3));
    };
};

int main(int argc, char* argv[]) {
    int exit_code = -1;

    try {
        maf::ActionFactory::Register<HelloWorldAction>();

        maf::barrier("===== TestBcastController controller;");

        TestBcastController controller;
        controller.start();

        maf::barrier("===== maf::BcastController controller2(GetActions(3));");

        maf::BcastController controller2(GetActions(3));
        controller2.start();

        maf::barrier("===== maf::ScatterController controller3(GetActions(3));");

        maf::ScatterController controller3(GetActions(3));
        controller3.start();
        exit_code = 0;
    }
    catch (std::exception& ex) {
        maf::log("FAILED: ", ex.what());
        MPI_Abort(MPI_COMM_WORLD, -1);
    }
    catch (...) {
        maf::log("FAILED: no idea what happened");
        MPI_Abort(MPI_COMM_WORLD, -1);
    }

    return exit_code;
}
