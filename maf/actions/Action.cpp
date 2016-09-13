#include <sstream>

#include "maf/actions/Action.hpp"

namespace maf {

    Action::Action() {
        this->transmitted = false;
    }

    void Action::serialize(std::shared_ptr<Archive> archive) {
        return;
    }

    void Action::tear_down() {
        return;
    }

    void Action::start() {
        this->run();
        this->tear_down();
    }

}
