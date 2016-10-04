#include "maf/actions/SplitMpiAction.hpp"
#include "maf/communication/MafComm.hpp"

namespace maf {

    SplitMpiAction::SplitMpiAction(int color, bool take_over)
        : Action(), _color(color), _take_over(take_over) {

    }

    SplitMpiAction::SplitMpiAction() : SplitMpiAction::SplitMpiAction(-1, false) {

    }

    void SplitMpiAction::run() {
        auto new_comm = MafComm::World->split(this->_color);

        if (this->_take_over) {
            new_comm->take_over_the_world();
        }
    }

    void SplitMpiAction::serialize(std::shared_ptr<Archive> archive) {
        (*archive) & this->_color;
        (*archive) & this->_take_over;
    }

    std::string SplitMpiAction::type_name() {
        return "SplitMpiAction";
    }

}
