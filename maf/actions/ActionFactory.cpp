#include "maf/actions/ActionFactory.hpp"
#include "maf/actions/EndLoopAction.hpp"
#include "maf/actions/EmptyAction.hpp"
#include "maf/actions/RevertSplitAction.hpp"
#include "maf/actions/SplitMpiAction.hpp"
#include "maf/controllers/ScatterController.hpp"
#include "maf/controllers/BcastController.hpp"

namespace maf {

    static bool _initialized_Factories = false;

    std::unordered_map<std::string, std::shared_ptr<ActionFactory>>& ActionFactory::_Factories() {
        static std::unordered_map<std::string, std::shared_ptr<ActionFactory>> factories;
        return factories;
    }

    std::vector<std::shared_ptr<ActionFactory>>& ActionFactory::_Tests() {
        static std::vector<std::shared_ptr<ActionFactory>> tests;
        return tests;
    }

    std::vector<std::string> ActionFactory::Names() {
        std::vector<std::string> names;

        for (auto key_val : ActionFactory::_Factories()) {
            names.push_back(key_val.first);
        }

        return names;
    }

    std::shared_ptr<Action> ActionFactory::Create(std::string name) {
        try {
            auto factory = ActionFactory::_Factories().at(name);
            auto action = factory->create_action();
            return action;
        }
        catch (std::out_of_range* ex) {
            std::ostringstream msg;
            msg << "No action with the name `" << name << "` has been registered:" << ex->what();
            throw Exception(msg.str());
        }
        catch (std::out_of_range& ex) {
            std::ostringstream msg;
            msg << "No action with the name `" << name << "` has been registered: " << ex.what();
            throw Exception(msg.str());
        }
        catch (...) {
            std::ostringstream msg;
            msg << "NO IDEA: No action with the name `" << name << "` has been registered.";
            throw Exception(msg.str());
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

        if (!_initialized_Factories) {
            auto end_loop = std::shared_ptr<ActionFactory>(new TActionFactory<EndLoopAction>("EndLoopAction"));
            auto scatter = std::shared_ptr<ActionFactory>(new TActionFactory<ScatterController>("ScatterController"));
            auto bcast = std::shared_ptr<ActionFactory>(new TActionFactory<BcastController>("BcastController"));
            auto empty = std::shared_ptr<ActionFactory>(new TActionFactory<EmptyAction>("EmptyAction"));
            auto split = std::shared_ptr<ActionFactory>(new TActionFactory<SplitMpiAction>("SplitMpiAction"));
            auto revert = std::shared_ptr<ActionFactory>(new TActionFactory<RevertSplitAction>("RevertSplitAction"));
            ActionFactory::_Factories()[end_loop->action_name] = end_loop;
            ActionFactory::_Factories()[scatter->action_name] = scatter;
            ActionFactory::_Factories()[bcast->action_name] = bcast;
            ActionFactory::_Factories()[empty->action_name] = empty;
            ActionFactory::_Factories()[split->action_name] = split;
            ActionFactory::_Factories()[revert->action_name] = revert;
            _initialized_Factories = true;
        }

        try {
            auto func = ActionFactory::_Factories().at(name);
            std::ostringstream msg;
            msg << "Action with the name `" << name << "` has already been registered.";
            throw Exception(msg.str());
        }
        catch (std::out_of_range& ex) {
            // success
            ActionFactory::_Factories()[name] = factory;
        }
    }

    ActionFactory::ActionFactory(std::string action_name)
        : action_name(action_name) {

    }
}
