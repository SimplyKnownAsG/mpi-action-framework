#include "mpi.h"

#include "maf/controllers/BcastController.hpp"
#include "maf/archives/ReadArchive.hpp"
#include "maf/archives/WriteArchive.hpp"
#include "maf/example.hpp"

namespace maf {

    std::shared_ptr<Action> BcastController::distribute(std::shared_ptr<Action> action) {
        if (action) {
            auto archive = new WriteArchive;
            std::string type_name = action->type_name();
            (*archive) & type_name;
            try {
                action->serialize(archive);
            }
            catch (...) {
                mpi_print("couldn't serialize, but whatever");
            }
            archive->bcast();
            return action;
        }
        else {
            auto archive = new ReadArchive;
            archive->bcast();
            std::string type_name;
            (*archive) & type_name;
            auto result = Action::Create(type_name);
            try {
                result->serialize(archive);
            }
            catch (...) {
                mpi_print("couldn't serialize, but whatever");
            }
            return result;
        }
    };

}
