#pragma once

#include <exception>
#include <string>
#include <sstream>
#include <typeinfo>

#ifdef _WIN32
#define NOEXCEPT
#else
#define NOEXCEPT noexcept
#endif

namespace maf {

    /// A general exception class
    class Exception : public std::exception {

    private:

        std::string _message;

        void update_message(std::exception& ex);

    public:

        /// Create an exception with a message
        Exception(std::string message);

        /// Create an exception from an std::exception.
        Exception(std::exception& exception);

        /// Create an exception with both a message, and in inner exception.
        Exception(std::string message, std::exception& inner_exception);

        /// Returns a string representation of the issue.
        const char* what() const NOEXCEPT override;

    };


#define CATCH_AND_THROW(msg) \
    catch (std::exception& ex) { \
        throw Exception(msg, ex); \
    } \
    catch (...) { \
        throw Exception((msg) + ": unknown error"); \
    }

}

