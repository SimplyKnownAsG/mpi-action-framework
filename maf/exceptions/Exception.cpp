#include "maf/exceptions/Exception.hpp"

namespace maf {

    void Exception::update_message(std::exception* ex) {
        std::istringstream inner_message(ex->what());
        std::string line;
        std::ostringstream stream;
        stream << std::endl
               << "  " << typeid(*ex).name();
        while (std::getline(inner_message, line)) {
            stream << std::endl
                   << "  " << line;
        }
        stream << std::endl
               << "  " << line;
        this->_message += stream.str();
    }

    Exception::Exception(std::string message) : _message(message) {
    }

    Exception::Exception(std::exception* exception) {
        std::ostringstream stream;
        stream << typeid(exception).name() << std::endl
               << exception->what();
        this->_message = stream.str();
    }

    Exception::Exception(std::string message, Exception* inner_exception) : _message(message) {
        this->update_message(inner_exception);
    }

    Exception::Exception(std::string message, std::exception* inner_exception) : _message(message) {
        this->update_message(inner_exception);
    }

    const char* Exception::what() {
        return this->_message.c_str();
    }

}
