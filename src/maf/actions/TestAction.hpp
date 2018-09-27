#pragma once

#include "maf/actions/AbstractTestAction.hpp"

namespace maf {

    template<class TAction>
    class TestAction : public AbstractTestAction {

    public:
        TestAction() : AbstractTestAction() {
        };

        ~TestAction() {
        };

        virtual std::string type_name() override {
            return typeid(TAction).name();
        };

    };

}
