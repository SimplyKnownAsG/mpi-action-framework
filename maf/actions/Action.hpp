#pragma once

#include "maf/macros.hpp"
#include "maf/archives/Archive.hpp"
#include "maf/example.hpp"

#include <unordered_map>
#include <memory>

namespace maf {

    // forward declaration
    class ActionFactory;

    class Action {

    private:
        bool transmitted;

        static std::unordered_map<std::string, std::shared_ptr<ActionFactory>> registerd_actions;

    public:

        static void Register(std::shared_ptr<ActionFactory> factory);

        static std::shared_ptr<Action> Create(std::string name);

        static std::vector<std::string> RegisteredNames();

        Action();

        virtual ~Action() {
        };

        virtual void run();

        virtual std::string type_name() = 0;

        virtual void serialize(Archive* archive);
    };

}
