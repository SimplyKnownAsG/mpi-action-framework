#include "maf/actions/RevertSplitAction.hpp"
#include "maf/MafComm.hpp"

namespace maf {
    RevertSplitAction::RevertSplitAction() : Action() {

    }

    void RevertSplitAction::run() {
        MafComm::World->prepare_for_tomorrow_night();
    }

    std::string RevertSplitAction::type_name() {
        return "RevertSplitAction";
    }

}
