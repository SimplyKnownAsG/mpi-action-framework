
#include "maf/archives/ReadArchive.hpp"
#include <string>

namespace maf {

    ReadArchive::ReadArchive() : Archive(), _stream() {

    }

    ReadArchive::ReadArchive(std::string msg) : Archive(), _stream(msg) {

    }

    ReadArchive::ReadArchive(const char* msg, int size) : ReadArchive() {
        std::string s_msg(msg, size);
        this->_stream.str(s_msg);
    }

    std::string ReadArchive::str() {
        return this->_stream.str();
    }

    size_t ReadArchive::length() {
        return this->_stream.tellg();
    }
}
