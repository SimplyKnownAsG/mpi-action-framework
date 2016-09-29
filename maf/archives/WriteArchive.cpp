
#include "maf/archives/WriteArchive.hpp"

namespace maf {

    WriteArchive::WriteArchive() : Archive(), _stream() {

    }

    std::string WriteArchive::str() {
        return this->_stream.str();
    }

    size_t WriteArchive::length() {
        return this->_stream.tellp();
    }

}
