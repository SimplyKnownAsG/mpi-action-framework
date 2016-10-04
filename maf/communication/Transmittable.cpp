#include "maf/communication/Transmittable.hpp"

namespace maf {

    std::string _Transmittable::type_name() {
        return "";
    }

    void _Transmittable::serialize(std::shared_ptr<Archive> archive) {
        return;
    }

}
