#pragma once

#include "maf/communication/Archive.hpp"
#include <sstream>

namespace maf {

    class ReadArchive : public Archive {
    private:
        std::istringstream _stream;

    public:
        ReadArchive();

        ReadArchive(std::string msg);

        ReadArchive(const char* msg, int size);

        CREATE_OPERATOR_AMP(
        override {
            this->_stream.read((char*)(&val), sizeof(val));
        },

        override {
            size_t size;
            this->_stream.read((char*)(&size), sizeof(size));
            val.resize(size);
            this->_stream.read((char*)(&val[0]), size);
        });

        std::string str() override;

        size_t length() override;
    };

}
