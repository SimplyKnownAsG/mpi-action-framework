#pragma once

#include "maf/actions/Action.hpp"

namespace maf {
    class ActionFactory {
    public:
        const std::string action_name;

        ActionFactory(std::string action_name);

        virtual ~ActionFactory() {
        };

        virtual std::shared_ptr<Action> create_action() = 0;
    };

    template<class TAction>
    class TActionFactory : ActionFactory {
    public:
        TActionFactory(std::string action_name) : ActionFactory(action_name) {

        };

        virtual std::shared_ptr<Action> create_action() {
            return std::shared_ptr<Action>(new TAction);
        };
    };
}
