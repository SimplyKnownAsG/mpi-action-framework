#include "maf/actions/EmptyAction.hpp"


namespace maf {
    EmptyAction::EmptyAction() : Action() {

    };

    EmptyAction::~EmptyAction() {

    }

    void EmptyAction::run() {
        // empty, as promised
    }

}
