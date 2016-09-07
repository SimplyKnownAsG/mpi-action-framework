#include "maf/actions/EndLoopAction.hpp"
#include "maf/example.hpp"

namespace maf {

    void EndLoopAction::run() {
        throw this->shared_from_this();
    }
}
