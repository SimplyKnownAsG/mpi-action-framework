#pragma once

#include "maf/actions/Action.hpp"
#include "maf/communication/Transmittable.hpp"

#define MAF_ACTION_SUBCLASS(action_type)                                                           \
    class action_type : public maf::Action, public maf::Transmittable<action_type>

#define MAF_REGISTER_ACTION(action_type)                                                           \
    bool action_type##_is_registerd = maf::ActionFactory::Register<action_type>();

#define MAF_ACTION(action_type)                                                                    \
    class action_type;                                                                             \
    MAF_REGISTER_ACTION(action_type)                                                               \
    MAF_ACTION_SUBCLASS(action_type) {                                                             \
        void run() override;                                                                       \
    };                                                                                             \
    void action_type::run()

#define MAF_TEST_ACTION(action_type)                                                               \
    class action_type;                                                                             \
    bool action_type##_is_registerd = maf::ActionFactory::Register<action_type>(true);             \
    class action_type : public maf::TestAction<action_type> {                                      \
        void run() override;                                                                       \
    };                                                                                             \
    void action_type::run()

#define MAF_SKIPPED_TEST(action_type)                                                              \
    class action_type;                                                                             \
    bool action_type##_is_registerd = maf::ActionFactory::Register<action_type>(true);             \
    class action_type : public maf::TestAction<action_type>

#define MAF_TEST_MAIN()                                                                            \
    int main(int argc, char* argv[]) {                                                             \
        int exit_code = -1;                                                                        \
        try {                                                                                      \
            auto controller = maf::TestController();                                               \
            controller.start();                                                                    \
            exit_code = 0;                                                                         \
        } catch (std::exception & ex) {                                                            \
            maf::log("FAILED: &", ex.what());                                                      \
        } catch (...) {                                                                            \
            maf::log("FAILED: no idea what happened");                                             \
        }                                                                                          \
        if (exit_code != 0) {                                                                      \
            MPI_Abort(MPI_COMM_WORLD, -1);                                                         \
        }                                                                                          \
        MPI_Finalize();                                                                            \
        return exit_code;                                                                          \
    }
