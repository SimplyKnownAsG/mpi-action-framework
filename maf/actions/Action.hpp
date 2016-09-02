#pragma once

#include "maf/macros.hpp"

namespace maf {

    class Action {

    private:
        bool transmitted;

    public:
        Action();

        virtual void set_up() EMPTY_RETURN;

        virtual void tear_down() EMPTY_RETURN;

        virtual void run() EMPTY_RETURN;

        void invoke();
    };

}
