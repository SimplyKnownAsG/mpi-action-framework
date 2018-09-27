#include "maf/actions/AbstractTestAction.hpp"

namespace maf {

    AbstractTestAction::AbstractTestAction() : Action(), _passed(true) {

    }

    AbstractTestAction::~AbstractTestAction() {

    }

    void AbstractTestAction::tear_down() {
        std::string msg = this->_passed ? "passed" : "failed";
        maf::barrier(this->type_name(), " ... ", msg);
    }

}
