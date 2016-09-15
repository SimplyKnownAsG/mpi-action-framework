#include "maf/actions/SplitMpiAction.hpp"

namespace maf {
    SplitMpiAction::SplitMpiAction(int color) : Action(), _color(color) {

    };

    SplitMpiAction::~SplitMpiAction() {

    }

    void SplitMpiAction::run() {
        auto this_p = shared_from_this();
        throw this_p;
    }

    std::string SplitMpiAction::type_name() {
        return "SplitMpiAction";
    }

}
