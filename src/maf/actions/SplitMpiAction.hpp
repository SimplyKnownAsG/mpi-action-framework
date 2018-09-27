#pragma once

#include "maf/actions/Action.hpp"
#include "maf/communication/Transmittable.hpp"

namespace maf {

    class SplitMpiAction : public Action, public Transmittable<SplitMpiAction> {
    private:
        int _color;
        bool _take_over;

    public:
        SplitMpiAction();

        SplitMpiAction(int color, bool take_over);

        void run() override;

        void serialize(std::shared_ptr<Archive> archive) override;

        std::string type_name() override;
    };

}
