#pragma once

#include "maf/actions/Action.hpp"
#include "maf/archives/Archive.hpp"
#include <memory>
#include <string>
#include <vector>

namespace maf {
    class ActionFactory {
    private:
        static std::unordered_map<std::string, std::shared_ptr<ActionFactory>> factories;

    public:

        static std::shared_ptr<Action> Create(std::string name);

        static std::shared_ptr<Action> Create(std::shared_ptr<Archive>& archive);

        static std::vector<std::string> Names();

        static void Register(std::shared_ptr<ActionFactory> factory);

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
