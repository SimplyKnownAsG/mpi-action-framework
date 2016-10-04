#pragma once

#include "maf/actions/Action.hpp"
#include "maf/communication/Archive.hpp"
#include "maf/controllers/TestController.hpp"

#include <memory>
#include <string>
#include <vector>
#include <iostream>

namespace maf {

    template<class TAction>
    class TActionFactory;

    class ActionFactory {
    private:
        friend class maf::TestController;

        static std::unordered_map<std::string, std::shared_ptr<ActionFactory>>& _Factories();

        static std::vector<std::shared_ptr<ActionFactory>>& _Tests();

    public:

        template <class TAction>
        static std::shared_ptr<Action> Create() {
            return ActionFactory::Create(typeid(TAction).name());
        };

        static std::shared_ptr<Action> Create(std::string name);

        static std::shared_ptr<Action> Create(std::shared_ptr<Archive>& archive);

        static std::vector<std::string> Names();

        static void Register(std::shared_ptr<ActionFactory> factory);

        template <class TAction>
        static bool Register(bool is_test = false) {
            std::string action_name = typeid(TAction).name();
            auto factory = std::shared_ptr<ActionFactory>(new TActionFactory<TAction>(action_name));
            ActionFactory::Register(factory);

            if (is_test) {
                ActionFactory::_Tests().push_back(factory);
            }

            return true;
        };

        const std::string action_name;

        ActionFactory(std::string action_name);

        virtual ~ActionFactory() {
        };

        virtual std::shared_ptr<Action> create_action() = 0;
    };

    template<class TAction>
    class TActionFactory : public ActionFactory {
    public:
        TActionFactory(std::string action_name) : ActionFactory(action_name) {

        };

        std::shared_ptr<Action> create_action() {
            return std::shared_ptr<Action>((Action*)new TAction);
        };
    };

}
