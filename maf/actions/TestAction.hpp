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

#define MAF_TEST_ACTION(action_type) \
    class action_type; \
    bool action_type## _is_registerd = maf::ActionFactory::Register< action_type >(true); \
    class action_type : public maf::TestAction< action_type >

}
