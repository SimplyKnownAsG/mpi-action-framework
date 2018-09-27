#include <sstream>

#include "maf/actions/Action.hpp"

namespace maf {

    Action::Action() : context(NULL) {
    }

    Action::~Action() {
    }

    void Action::set_up() {
        return;
    }

    void Action::tear_down() {
        return;
    }

    void Action::start(std::shared_ptr<Context> context) {
        this->context = context;
        this->set_up();
        this->run();
        this->tear_down();
    }

}
