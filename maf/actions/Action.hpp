#pragma once

#include "maf/archives/Archive.hpp"
#include "maf/Context.hpp"

#include <unordered_map>
#include <memory>
#include <vector>

namespace maf {

    class Action : public std::enable_shared_from_this<Action> {

    public:

        std::shared_ptr<Context> context;

        Action();

        virtual ~Action();

        virtual void run() = 0;

        virtual void start(std::shared_ptr<Context> context = NULL);

        virtual void serialize(std::shared_ptr<Archive> archive);

        virtual void tear_down();

        virtual std::string type_name() = 0;
    };

}
