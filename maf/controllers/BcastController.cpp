#include "mpi.h"

#include "maf/controllers/BcastController.hpp"
#include "maf/archives/ReadArchive.hpp"
#include "maf/archives/WriteArchive.hpp"

namespace maf {

    Action* BcastController::distribute(Action* act) {
        Archive* archive = NULL;
        std::string type_name;
        if (act) {
            archive = new WriteArchive();
            type_name = act->type_name();
            (*archive) & type_name;
            act->serialize(archive);
            archive->bcast();
        }
        else {
            archive = new ReadArchive();
            archive->bcast();
            (*archive) & type_name;
            act = Action::Create(type_name);
            act->serialize(archive);
        }
        return act;
    };

}
