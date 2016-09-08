#include <sstream>

#include "maf/actions/Action.hpp"

namespace maf {

    Action::Action() {
        this->transmitted = false;
    }

    void Action::run() {
        return;
    }

    void Action::serialize(std::shared_ptr<Archive> archive) {
        return;
    }

    void Action::tear_down() {
        return;
    }

}
