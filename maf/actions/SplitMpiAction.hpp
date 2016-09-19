#pragma once

#include "maf/actions/Action.hpp"

namespace maf {

    class SplitMpiAction : public Action {
    private:
        int _color;

    public:
        SplitMpiAction(int color = -1);

        ~SplitMpiAction();

        void run() override;

        std::string type_name() override;
    };

}
