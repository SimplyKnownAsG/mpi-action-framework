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
// 	virtual void operator&(std::vector<int> &val) body; \
// 	virtual void operator&(std::vector<long long> &val) body; \
// 	virtual void operator&(std::vector<float> &val) body; \
// 	virtual void operator&(std::vector<double> &val) body; \
// 	virtual void operator&(std::string &val) body; \
    
#define CREATE_METHODS(method, body) \
	int method(int &val) body; \
	long long method(long long &val) body; \
	float method(float &val) body; \
	double method(double &val) body;

#define SERIALIZE_BODY() \
    (*this)& val; \
    return val;

    class Archive {

    public:

        Archive() {
        };

        CREATE_OPERATOR_AMP(= 0, = 0);

        // CREATE_OPERATOR_AMP_VECTOR(= 0);

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

}
