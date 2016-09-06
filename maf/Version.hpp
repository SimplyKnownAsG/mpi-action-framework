#pragma once

#include <string>

namespace maf {

    class Version {
    public:
        static const std::string git_sha1;

        static const std::string git_description;
    };

}