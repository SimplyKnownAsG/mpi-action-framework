#pragma once

#include <memory>

namespace maf {

    class Context : public std::enable_shared_from_this<Context> {

    public:

        Context();

        virtual ~Context();

        template <class TContext>
        TContext* as() {
            return (TContext*)this;
        };

        Context* ptr();

    };

}
