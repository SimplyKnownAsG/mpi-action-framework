#pragma once

#include "maf/actions/Action.hpp"
#include "maf/Log.hpp"
#include "maf/exceptions/Exception.hpp"
#include <sstream>
#include <cmath>

namespace maf {

    class AbstractTestAction : public Action {
    private:
        bool _passed;

    public:
        AbstractTestAction() : Action(), _passed(true) {
        };

        virtual ~AbstractTestAction() {

        };

        void tear_down() {
            std::string msg = this->_passed ? "passed" : "failed";
            maf::barrier(this->type_name(), "...", msg);
        };

        template<class T1, class T2>
        void assert_equal(T1 expected, T2 actual, std::string prefix = "AssertionError: ") {
            this->_passed = (actual == expected);

            if (!this->_passed) {
                std::ostringstream msg;
                msg << prefix << "expected (" << expected << ") != actual (" << actual << ")";
                maf::warning(msg.str());
                throw new maf::Exception(msg.str());
            }
        };

        template<class T1, class T2>
        void assert_not_equal(T1 expected, T2 actual, std::string prefix = "AssertionError: ") {
            this->_passed = (actual != expected);

            if (!this->_passed) {
                std::ostringstream msg;
                msg << prefix << "expected (" << expected << ") == actual (" << actual << ")";
                maf::warning(msg.str());
                throw new maf::Exception(msg.str());
            }
        };

        template<class T>
        void assert_close(T expected, T actual, T relative, std::string prefix = "AssertionError: ") {
            T error = std::abs((actual - expected) / expected);
            this->_passed = error <= relative;

            if (!this->_passed) {
                std::ostringstream msg;
                msg << prefix << "(actual - expected) / expected > relative\n"
                    << "( (" << actual << ") - (" << expected << ") ) / (" << expected << ") = ("
                    << error << ") > " << relative;
                maf::warning(msg.str());
                throw new maf::Exception(msg.str());
            }
        };

        template<class T>
        void assert_not_close(T expected, T actual, T relative, std::string prefix = "AssertionError: ") {
            T error = std::abs((actual - expected) / expected);
            this->_passed =  error > relative;

            if (!this->_passed) {
                std::ostringstream msg;
                msg << prefix << "(actual - expected) / expected > relative\n"
                    << "( (" << actual << ") - (" << expected << ") ) / (" << expected << ") = ("
                    << error << ") <= " << relative;
                maf::warning(msg.str());
                throw new maf::Exception(msg.str());
            }
        };
    };

}
