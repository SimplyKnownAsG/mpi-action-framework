#include "maf/communication/Transmittable.hpp"
#include "maf/exceptions/Exception.hpp"

namespace maf {

    _Transmittable::~_Transmittable() {

    }

    std::string _Transmittable::type_name() {
        throw Exception("Must override _Transmittable::type_name()");
    }

    void _Transmittable::serialize(std::shared_ptr<Archive> archive) {
        return;
    }

}
