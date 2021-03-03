//
// Created by masa_ on 16.09.2020.
//

#ifndef LONG_NUMBERS_LIBRARY_EXCEPTION_H_
#define LONG_NUMBERS_LIBRARY_EXCEPTION_H_

#include <string>

namespace long_number {
    struct exception {
     public:
        std::string message;
        int code;

        exception(std::string message, int code) {
            this->message = message;
            this->code = code;
        }
    };
}

#endif //LONG_NUMBERS_LIBRARY_EXCEPTION_H_
