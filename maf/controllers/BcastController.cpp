#include "mpi.h"

#include "maf/controllers/BcastController.hpp"
#include "maf/archives/ReadArchive.hpp"
#include "maf/archives/WriteArchive.hpp"
#include "maf/actions/ActionFactory.hpp"

namespace maf {

    BcastController::BcastController() : Controller() {
        // empty;
    }

    std::shared_ptr<Action> BcastController::distribute(std::shared_ptr<Action> action) {
        if (action) {
            std::shared_ptr<Archive> archive = std::shared_ptr<Archive>(new WriteArchive);
            std::string type_name = action->type_name();
            (*archive) & type_name;
            action->serialize(archive);
            archive->bcast();
            return action;
        }
        else {
            std::shared_ptr<Archive> archive = std::shared_ptr<Archive>(new ReadArchive);
            archive->bcast();
            std::string type_name;
            (*archive) & type_name;
            std::shared_ptr<Action> result = ActionFactory::Create(type_name);
            result->serialize(archive);
            return result;
        }
    }
    
    std::shared_ptr<Action> BcastController::scatter(std::vector<std::shared_ptr<Action>> actions) {
        // we will use actions as a stack
        return NULL;
    }

}
