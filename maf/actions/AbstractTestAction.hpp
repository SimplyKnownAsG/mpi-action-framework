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

        AbstractTestAction();

        virtual ~AbstractTestAction();

        void tear_down();

        template<class T>
        void assert_equal(const T& expected, const T& actual, std::string prefix = "AssertionError: ") {
            this->_assert_equal(expected, actual, static_cast<T*>(0), prefix);
        };

    private:

        template<class T>
        void _assert_equal(const T& expected, const T& actual, T* _ignored, std::string prefix) {
            this->_passed = (actual == expected);

            if (!this->_passed) {
                std::ostringstream msg;
                msg << prefix << "expected (" << expected << ") != actual (" << actual << ")";
                maf::warning(msg.str());
                throw maf::Exception(msg.str());
            }
        };

        template<class T>
        void _assert_equal(const std::vector<T>& expected, const std::vector<T>& actual, std::vector<T>* _ignored, std::string prefix) {
            this->_passed = (expected.size() == actual.size());
            if (!this->_passed) {
                std::ostringstream msg;
                msg << prefix << "expected.size() (" << expected.size() << ") != actual.size() (" << actual.size() << ")";
                maf::warning(msg.str());
                throw maf::Exception(msg.str());
            }

            for (size_t ii = 0; ii < expected.size(); ii++) {
                this->_passed = expected[ii] == actual[ii];

                if (!this->_passed) {
                    break;
                }
            }

            if (!this->_passed) {
                std::ostringstream msg;
                msg << prefix << "expected != actual" << std::endl;
                std::string vec_name = "expected: ";

                for (std::vector<T>& vec : std::vector<std::vector<T>> {expected, actual}) {
                    msg << vec_name;

                    for (auto val : vec) {
                        msg << val << ", ";
                    }

                    msg << std::endl;
                    vec_name = "actual: ";
                }
                maf::warning(msg.str());
                throw maf::Exception(msg.str());
            }
        };

    public:
        template<class T>
        void assert_not_equal(const T& expected, const T& actual, std::string prefix = "AssertionError: ") {
            this->_assert_not_equal(expected, actual, static_cast<T*>(0), prefix);
        };

    private:
        template<class T>
        void _assert_not_equal(T expected, T actual, T* _ignored, std::string prefix) {
            this->_passed = (actual != expected);

            if (!this->_passed) {
                std::ostringstream msg;
                msg << prefix << "expected (" << expected << ") == actual (" << actual << ")";
                maf::warning(msg.str());
                throw maf::Exception(msg.str());
            }
        };


        template<class T>
        void _assert_not_equal(const std::vector<T>& expected, const std::vector<T>& actual, std::vector<T>* _ignored, std::string prefix) {
            this->_passed = expected.size() != actual.size();
            if (this->_passed) {
                return;
            }

            for (size_t ii = 0; ii < expected.size(); ii++) {
                if (expected[ii] != actual[ii]) {
                    this->_passed = true;
                    break;
                }
            }

            if (!this->_passed) {
                std::ostringstream msg;
                msg << prefix << "expected == actual" << std::endl;
                std::string vec_name = "expected: ";

                for (std::vector<T>& vec : std::vector<std::vector<T>> {expected, actual}) {
                    msg << vec_name;

                    for (auto val : vec) {
                        msg << val << ", ";
                    }

                    msg << std::endl;
                    vec_name = "actual: ";
                }
                maf::warning(msg.str());
                throw maf::Exception(msg.str());
            }
        };

    public:

        template<class T1, class T2>
        void assert_close(T1 expected, T1 actual, T2 relative, std::string prefix = "AssertionError: ") {
            this->_assert_close(expected, actual, relative, static_cast<T1*>(0), prefix);
        };

    private:

        template<class T>
        void _assert_close(T expected, T actual, T relative, T* _ignored, std::string prefix) {
            T error = std::abs((actual - expected) / expected);
            this->_passed = error <= relative;

            if (!this->_passed) {
                std::ostringstream msg;
                msg << prefix << "(actual - expected) / expected > relative\n"
                    << "( (" << actual << ") - (" << expected << ") ) / (" << expected << ") = ("
                    << error << ") > " << relative;
                maf::warning(msg.str());
                throw maf::Exception(msg.str());
            }
        };

        template<class T>
        void _assert_close(std::vector<T> expected, std::vector<T> actual, T relative, std::vector<T>* _ignored, std::string prefix) {
            this->_passed = (expected.size() == actual.size());
            if (!this->_passed) {
                std::ostringstream msg;
                msg << prefix << "expected.size() (" << expected.size() << ") != actual.size() (" << actual.size() << ")";
                maf::warning(msg.str());
                throw maf::Exception(msg.str());
            }

            for (size_t ii = 0; ii < expected.size(); ii++) {
                this->_passed = (std::abs(expected[ii] - actual[ii]) / expected[ii]) < relative;

                if (!this->_passed) {
                    break;
                }
            }

            if (!this->_passed) {
                std::ostringstream msg;
                msg << prefix << "expected != actual" << std::endl;
                std::string vec_name = "expected: ";

                for (std::vector<T>& vec : std::vector<std::vector<T>> {expected, actual}) {
                    msg << vec_name;

                    for (auto val : vec) {
                        msg << val << ", ";
                    }

                    msg << std::endl;
                    vec_name = "actual: ";
                }
                maf::warning(msg.str());
                throw maf::Exception(msg.str());
            }
        };


    public:

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
                throw maf::Exception(msg.str());
            }
        };
    };

}
