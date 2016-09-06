#pragma once

#include <unordered_map>
#include <vector>

#include "maf/macros.hpp"

namespace maf {

    class Action {

    private:
        bool transmitted;

        static std::unordered_map<std::string, Action*(*)()> registerd_actions;

        static std::unordered_map<std::string, void(*)(Action*)> update_functions;

    public:

        static void Register(std::string name, Action*(*create_func)());

        static void Register(std::string name, void (*create_func)(Action*));

        static std::vector<std::string> RegisteredNames();

        Action();

        virtual void set_up() { return; };

        virtual void tear_down() { return; };

        virtual void run() { return; };

        virtual const std::string type_name() {
            return "Action";
        }

        void invoke();
    };

    template <class T> Action* create_action() {
        return new T;
    };

}
