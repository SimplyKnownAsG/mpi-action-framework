#include "maf/actions/ActionFactory.hpp"
#include "maf/actions/EndLoopAction.hpp"
#include "maf/controllers/ScatterController.hpp"
#include "maf/controllers/BcastController.hpp"

namespace maf {
    
    std::unordered_map<std::string, std::shared_ptr<ActionFactory>> ActionFactory::factories {
        {"EndLoopAction", std::shared_ptr<ActionFactory>((ActionFactory*)(new TActionFactory<EndLoopAction>("EndLoopAction")))},
        {"ScatterController", std::shared_ptr<ActionFactory>((ActionFactory*)(new TActionFactory<ScatterController>("ScatterController")))},
        {"BcastController", std::shared_ptr<ActionFactory>((ActionFactory*)(new TActionFactory<BcastController>("BcastController")))}
    };

    std::vector<std::string> ActionFactory::Names() {
        std::vector<std::string> names;
        for (auto key_val : ActionFactory::factories) {
            names.push_back(key_val.first);
        }
        return names;
    }

    std::shared_ptr<Action> ActionFactory::Create(std::string name) {
        try {
            auto factory = ActionFactory::factories.at(name);
            return factory->create_action();
        }
        catch (std::out_of_range* ex) {
            std::ostringstream msg;
            msg << "No action with the name `" << name << "` has been registered.";
            throw new Exception(msg.str());
        }
    }

    std::shared_ptr<Action> ActionFactory::Create(std::shared_ptr<Archive>& archive) {
        std::string name;
        (*archive) & name;
        auto action = ActionFactory::Create(name);
        action->serialize(archive);
    }

    void ActionFactory::Register(std::shared_ptr<ActionFactory> factory) {
        auto name = factory->action_name;
        try {
            auto func = ActionFactory::factories.at(name);
            std::ostringstream msg;
            msg << "Action with the name `" << name << "` has already been registered.";
            throw new Exception(msg.str());
        }
        catch (std::out_of_range& ex) {
            // success
            ActionFactory::factories[name] = factory;
        }
    }

    ActionFactory::ActionFactory(std::string action_name)
        : action_name(action_name) {

    }
}
