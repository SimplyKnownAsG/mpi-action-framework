#pragma once

#include <mpi.h>
#include <vector>
#include <string>
#include <iostream>
#include <istream>
#include <sstream>

namespace maf {
    void mpi_print(std::string msg);

#define CREATE_OPERATOR_AMP(body) \
	virtual void operator&(int &val) body; \
	virtual void operator&(long long &val) body; \
	virtual void operator&(float &val) body; \
	virtual void operator&(double &val) body;

#define CREATE_METHODS(method, body) \
	int method(int &val) body; \
	long long method(long long &val) body; \
	float method(float &val) body; \
	double method(double &val) body;

#define SERIALIZE_BODY() \
    (*this)& val; \
    return val;

    class IArchive {
    public:
        IArchive() {
        };

        CREATE_OPERATOR_AMP(= 0);

        int serialize_int(int val) {
            SERIALIZE_BODY()
        };

        long long serialize_long_long(long long val) {
            SERIALIZE_BODY()
        };

        float serialize_float(float val) {
            SERIALIZE_BODY()
        };

        double serialize_double(double val) {
            SERIALIZE_BODY()
        };

        virtual void bcast(int root=0) = 0;
    };


    class ReadArchive : public IArchive {
    private:
        std::istringstream _stream;

    public:
        ReadArchive() : IArchive(), _stream() {

        };
        ReadArchive(std::string msg) : IArchive(), _stream(msg) {

        };

        CREATE_OPERATOR_AMP(override {
            this->_stream.read((char*)(&val), sizeof(val));
        });

        void bcast(int root=0) {
            int size;
            MPI_Bcast(&size, sizeof(int), MPI_INT, root, MPI_COMM_WORLD);
            char *buffer = new char[size];
            MPI_Bcast((void *)buffer, size * sizeof(char), MPI_CHAR, root, MPI_COMM_WORLD);
            std::string s( buffer, size ) ;
            this->_stream.str(s);
            delete buffer;
        };
    };

    class WriteArchive : public IArchive {
    private:
        std::ostringstream _stream;

    public:
        WriteArchive() : IArchive(), _stream() {
        };

        CREATE_OPERATOR_AMP(override {
            this->_stream.write((char*)(&val), sizeof(val));
        });

        void bcast(int root=0) {
            std::string buffer_as_string = this->_stream.str();
            const char *buffer = buffer_as_string.c_str();
            int size = buffer_as_string.size();
            MPI_Bcast(&size, sizeof(size), MPI_INT, root, MPI_COMM_WORLD);
            MPI_Bcast((void *)buffer, size * sizeof(char), MPI_CHAR, root, MPI_COMM_WORLD);
            // delete buffer; not needed -- buffer_as_string gets deleted when it goes out of scope
        };
    };


    class Thing
    {
    private:
        static int count;
    public:
        int index;
        std::string name;
        Thing(std::string name) : name(name)
        {
            this->index = count++;
        };

        virtual void serialize(IArchive *serializer) {
            return;
        };

        void bcast(int root=0) {
            int rank;
            MPI_Comm_rank(MPI_COMM_WORLD, &rank);
            IArchive *serializer = NULL;
            if (rank == root) {
                serializer = new WriteArchive();
                this->serialize(serializer);
                serializer->bcast();
            }
            else {
                serializer = new ReadArchive();
                serializer->bcast();
                this->serialize(serializer);
            }
            delete serializer;
        };
    };
}