#include "maf/maf.hpp"

#include <mpi.h>


class AnnoyContext : public maf::Context {

private:
    int annoyance_count = 0;

public:

    void annoy() {
        if (++this->annoyance_count > 2) {
            maf::log("Ugh, so annoyed!!! ", annoyance_count);
        }
        else {
            maf::log("Nope, not annoyed... ", annoyance_count);
        }
    };

};


class AnnoyAction : public maf::Action {

public:

    void run() override {
        maf::log("Are you annoyed yet?");
        this->context->as<AnnoyContext>()->annoy();
    };

    std::string type_name() {
        return "AnnoyAction";
    }

};


int main(int argc, char* argv[]) {
    int exit_code = -1;

    try {
        auto factory = std::shared_ptr<maf::ActionFactory>(new maf::TActionFactory<AnnoyAction>("AnnoyAction"));
        maf::ActionFactory::Register(factory);

        auto context = std::shared_ptr<maf::Context>(new AnnoyContext);
        std::vector<std::shared_ptr<maf::Action>> actions(3, std::shared_ptr<maf::Action>(new AnnoyAction));

        maf::BcastController controller(actions);
        controller.start(context);

        exit_code = 0;
    }
    catch (std::exception* ex) {
        maf::log("FAILED: ", ex->what());
    }
    catch (...) {
        maf::log("FAILED: no idea what happened");
    }

    MPI_Finalize();
    return exit_code;
}
