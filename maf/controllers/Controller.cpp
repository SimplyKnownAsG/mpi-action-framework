
#include "maf/controllers/Controller.hpp"
#include "maf/actions/EndLoopAction.hpp"

namespace maf {

    void Controller::start() {
        try {
            while (true) {
                Action *action = this->get_action();
                action->invoke();
                delete action;
            }
        }
        catch (EndLoopAction *end) {
            /* success */
        }
    }

}
