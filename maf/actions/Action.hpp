#pragma once

#include "maf/communication/Transmittable.hpp"
#include "maf/Context.hpp"

#include <unordered_map>
#include <memory>
#include <vector>

namespace maf {

    class Action : public virtual _Transmittable, public std::enable_shared_from_this<Action> {

    public:

        std::shared_ptr<Context> context;

        Action();

        virtual ~Action();

        virtual void run() = 0;

        virtual void set_up();

        virtual void start(std::shared_ptr<Context> context = NULL);

        virtual void tear_down();
    };

}
