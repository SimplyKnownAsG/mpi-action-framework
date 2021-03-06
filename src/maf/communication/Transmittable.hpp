#pragma once

#include "maf/communication/Archive.hpp"

#include <string>
#include <memory>

namespace maf {

    class _Transmittable {

    public:
        virtual ~_Transmittable();

        virtual std::string type_name();

        virtual void serialize(std::shared_ptr<Archive> archive);
    };

    template <class T>
    class Transmittable : public virtual _Transmittable {

    public:

        std::string type_name() override {
            return typeid(T).name();
        };
    };

}