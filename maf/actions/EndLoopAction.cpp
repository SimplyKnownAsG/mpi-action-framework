#include "maf/actions/EndLoopAction.hpp"

namespace maf {

    void EndLoopAction::invoke() {
        throw this;
    }

}
