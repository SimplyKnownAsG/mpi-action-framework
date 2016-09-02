#include "maf/actions/Action.hpp"

namespace maf {

    Action::Action() {
        this->transmitted = false;
    }

    void Action::invoke() {
        this->set_up();
        this->run();
        this->tear_down();
    };

}
