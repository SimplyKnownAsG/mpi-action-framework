#pragma once

#include "maf/archives/Archive.hpp"
#include <string>
#include <sstream>

namespace maf {

    class WriteArchive : public Archive {
    private:
        std::ostringstream _stream;

    public:
        WriteArchive();

        CREATE_OPERATOR_AMP(override {
            this->_stream.write((char*)(&val), sizeof(val));
        },
        
        override {
            size_t size = val.size();
            this->_stream.write((char*)(&size), sizeof(size));
            this->_stream.write((char*)(&val[0]), size);
        });

        void bcast(int root=0);
    };

}