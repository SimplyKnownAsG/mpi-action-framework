#pragma once

#include <string>

namespace maf {

#define CREATE_OPERATOR_AMP(body, string_body) \
    virtual void operator&(int &val) body; \
    virtual void operator&(long long &val) body; \
    virtual void operator&(float &val) body; \
    virtual void operator&(double &val) body; \
    virtual void operator&(std::string &val) string_body;

// #define CREATE_OPERATOR_AMP_VECTOR(body) \
//  virtual void operator&(std::vector<int> &val) body; \
//  virtual void operator&(std::vector<long long> &val) body; \
//  virtual void operator&(std::vector<float> &val) body; \
//  virtual void operator&(std::vector<double> &val) body; \
//  virtual void operator&(std::string &val) body; \

#define CREATE_METHODS(method, body) \
    int method(int &val) body; \
    long long method(long long &val) body; \
    float method(float &val) body; \
    double method(double &val) body;

#define RW_BODY() \
    (*this)& val; \
    return val;

    class Archive {

    public:

        Archive() {
        };

        CREATE_OPERATOR_AMP(= 0, = 0);

        // CREATE_OPERATOR_AMP_VECTOR(= 0);

        int rw_int(int val) {
            RW_BODY()
        };

        long long rw_long_long(long long val) {
            RW_BODY()
        };

        float rw_float(float val) {
            RW_BODY()
        };

        double rw_double(double val) {
            RW_BODY()
        };

        std::string rw_string(std::string val) {
            RW_BODY()
        };

        virtual void bcast(int root=0) = 0;

        virtual std::string str() = 0;

        virtual size_t length() = 0;

    };

}
