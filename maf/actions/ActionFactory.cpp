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

    void ActionFactory::Register(std::shared_ptr<ActionFactory> factory, bool is_test) {
        auto name = factory->action_name;

        if (!_initialized_Factories) {
            _initialized_Factories = true;
            ActionFactory::Register<EndLoopAction>();
            ActionFactory::Register<ScatterController>();
            ActionFactory::Register<BcastController>();
            ActionFactory::Register<EmptyAction>();
            ActionFactory::Register<SplitMpiAction>();
            ActionFactory::Register<RevertSplitAction>();
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

        if (is_test) {
            ActionFactory::_Tests().push_back(factory);
        }
    }

    ActionFactory::ActionFactory(std::string action_name)
        : action_name(action_name) {

    }
}
