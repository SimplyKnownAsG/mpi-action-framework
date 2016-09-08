#include "maf/actions/EndLoopAction.hpp"

namespace maf {

    void EndLoopAction::run() {
        throw this->shared_from_this();
    }
}
