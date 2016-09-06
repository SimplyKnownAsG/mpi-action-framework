#include <sstream>

#include "maf/actions/Action.hpp"
#include "maf/actions/EndLoopAction.hpp"
#include "maf/exceptions/Exception.hpp"

namespace maf {

    std::unordered_map<std::string, Action*(*)()> Action::registerd_actions {
        {"EndLoopAction", &create_action<EndLoopAction> }
    };

    std::unordered_map<std::string, void (*)(Action*)> Action::update_functions;

    void Action::Register(std::string name, Action*(*create_func)()) {
        try {
            auto func = Action::registerd_actions.at(name);
            std::ostringstream msg;
            msg << "Action with the name `" << name << "` has already been registered.";
            throw new Exception(msg.str());
        }
        catch (std::out_of_range& ex) {
            // success
            Action::registerd_actions[name] = create_func;
        }
    };

    void Action::Register(std::string name, void (*update_func)(Action*)) {
        try {
            auto func = Action::update_functions.at(name);
            std::ostringstream msg;
            msg << "Update function with the name `" << name << "` has already been registered.";
            throw new Exception(msg.str());
        }
        catch (std::out_of_range& ex) {
            // success
            Action::update_functions[name] = update_func;
        }
    };

    std::vector<std::string> Action::RegisteredNames() {
        std::vector<std::string> names;
        for (auto key_val : Action::registerd_actions) {
            names.push_back(key_val.first);
        }
        return names;
    };

    Action::Action() {
        this->transmitted = false;
    }

    void Action::invoke() {
        this->set_up();
        this->run();
        this->tear_down();
    };

}
