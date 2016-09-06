#include <sstream>

#include "maf/actions/Action.hpp"
#include "maf/actions/EndLoopAction.hpp"
#include "maf/exceptions/Exception.hpp"

namespace maf {

    std::unordered_map<std::string, Action*(*)()> Action::registerd_actions {
        {"EndLoopAction", &create_action<EndLoopAction> }
    };

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

    Action* Action::Create(std::string name) {
        try {
            auto func = Action::registerd_actions.at(name);
            return func();
        }
        catch (std::out_of_range& ex) {
            std::ostringstream msg;
            msg << "No action with the name `" << name << "` has been registered.";
            throw new Exception(msg.str());
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

}
