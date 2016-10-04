#pragma once

#include "maf/communication/Transmittable.hpp"
#include "maf/actions/Action.hpp"

#define MAF_ACTION_SUBCLASS(action_type) \
    class action_type : public maf::Action, public maf::Transmittable<action_type>


#define MAF_REGISTER_ACTION(action_type) \
    bool action_type## _is_registerd = maf::ActionFactory::Register<action_type>();


#define MAF_REGISTER_ACTION_SUBCLASS(action_type) \
    class action_type; \
    MAF_REGISTER_ACTION(action_type) \
    MAF_ACTION_SUBCLASS(action_type)


#define MAF_TEST_ACTION(action_type) \
    class action_type; \
    bool action_type## _is_registerd = maf::ActionFactory::Register< action_type >(true); \
    class action_type : public maf::TestAction< action_type >
