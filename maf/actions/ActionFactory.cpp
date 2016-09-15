#include "maf/actions/ActionFactory.hpp"
#include "maf/actions/EndLoopAction.hpp"
#include "maf/actions/EmptyAction.hpp"
#include "maf/controllers/ScatterController.hpp"
#include "maf/controllers/BcastController.hpp"

namespace maf {

    static std::unordered_map<std::string, std::shared_ptr<ActionFactory>> initialize_factories() {
        std::unordered_map<std::string, std::shared_ptr<ActionFactory>> factories;
        auto end_loop = std::shared_ptr<ActionFactory>(new TActionFactory<EndLoopAction>("EndLoopAction"));
        auto scatter = std::shared_ptr<ActionFactory>(new TActionFactory<ScatterController>("ScatterController"));
        auto bcast = std::shared_ptr<ActionFactory>(new TActionFactory<BcastController>("BcastController"));
        auto empty = std::shared_ptr<ActionFactory>(new TActionFactory<EmptyAction>("EmptyAction"));
        factories[end_loop->action_name] = end_loop;
        factories[scatter->action_name] = scatter;
        factories[bcast->action_name] = bcast;
        factories[empty->action_name] = empty;
        return factories;
    }

    std::unordered_map<std::string, std::shared_ptr<ActionFactory>> ActionFactory::factories = initialize_factories();

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
            auto action = factory->create_action();
            return action;
        }
        catch (std::out_of_range* ex) {
            std::ostringstream msg;
            msg << "No action with the name `" << name << "` has been registered:" << ex->what();
            throw new Exception(msg.str());
        }
        catch (std::out_of_range& ex) {
            std::ostringstream msg;
            msg << "No action with the name `" << name << "` has been registered: " << ex.what();
            throw new Exception(msg.str());
        }
        catch (...) {
            std::ostringstream msg;
            msg << "NO IDEA: No action with the name `" << name << "` has been registered.";
            throw new Exception(msg.str());
        }
    }

    std::shared_ptr<Action> ActionFactory::Create(std::shared_ptr<Archive>& archive) {
        std::string name;
        (*archive) & name;
        auto action = ActionFactory::Create(name);
        action->serialize(archive);
        return action;
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
