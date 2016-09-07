#include <sstream>

#include "maf/actions/Action.hpp"
#include "maf/actions/EndLoopAction.hpp"
#include "maf/exceptions/Exception.hpp"
#include "maf/actions/ActionFactory.hpp"

namespace maf {

    std::unordered_map<std::string, std::shared_ptr<ActionFactory>> Action::registerd_actions {
        {"EndLoopAction", std::shared_ptr<ActionFactory>((ActionFactory*)(new TActionFactory<EndLoopAction>("EndLoopAction")))}
    };

    void Action::Register(std::shared_ptr<ActionFactory> factory) {
        auto name = factory->action_name;
        try {
            auto func = Action::registerd_actions.at(name);
            std::ostringstream msg;
            msg << "Action with the name `" << name << "` has already been registered.";
            throw new Exception(msg.str());
        }
        catch (std::out_of_range& ex) {
            // success
            Action::registerd_actions[name] = factory;
        }
    };

    std::shared_ptr<Action> Action::Create(std::string name) {
        try {
            auto factory = Action::registerd_actions.at(name);
            auto result = factory->create_action();
            return result;
        }
        catch (std::out_of_range* ex) {
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

    void Action::run() {
        return;
    }

    void Action::serialize(Archive* archive) {
        return;
    }

}
