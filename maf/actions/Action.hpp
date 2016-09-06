#pragma once

#include <unordered_map>
#include <vector>

#include "maf/macros.hpp"
#include "maf/archives/Archive.hpp"

namespace maf {

    class Action {

    private:
        bool transmitted;

        static std::unordered_map<std::string, Action*(*)()> registerd_actions;

    public:

        static void Register(std::string name, Action*(*create_func)());

        static Action * Create(std::string name);

        static std::vector<std::string> RegisteredNames();

        Action();

        virtual void run() { return; };

        virtual std::string type_name() = 0;

        virtual void serialize(Archive *archive) { return; };
    };

    template <class T> Action* create_action() {
        return new T;
    };

}
