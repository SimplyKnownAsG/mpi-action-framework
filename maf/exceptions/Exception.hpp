#pragma once

#include <exception>
#include <string>
#include <sstream>
#include <typeinfo>

namespace maf {

    /// A general exception class
    class Exception : public std::exception {

    private:

        std::string _message;

        void update_message(std::exception* ex);

    public:

        /// Create an exception with a message
        Exception(std::string message);

        /// Create an exception from an std::exception.
        Exception(std::exception* exception);

        /// Create an exception with both a message, and in inner exception.
        Exception(std::string message, Exception* inner_exception);

        /// Create an exception with both a message, and in inner exception.
        Exception(std::string message, std::exception* inner_exception);

        /// Returns a string representation of the issue.
        const char* what();

    };


#define CATCH_AND_THROW(msg) \
    catch (Exception *ex) { \
        throw Exception(msg, ex); \
    } \
    catch (Exception &ex) { \
        throw Exception(msg, &ex); \
    } \
    catch (std::exception *ex) { \
        std::string message = msg; \
        throw Exception(message, ex); \
    } \
    catch (std::string &ex) { \
        std::string message = msg; \
        message += ": "; \
        message += ex; \
        throw Exception(message); \
    } \
    catch (...) { \
        std::string message = msg; \
        message += ": unknown error"; \
        throw Exception(message); \
    }

}

