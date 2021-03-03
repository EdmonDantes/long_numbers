//
// Created by masa_ on 12.09.2020.
//
#include "convert_util.h"
namespace long_number {
    namespace convert_util {
        std::string uIntToString(uintmax number, uintmax numberSystem) {
            if (number >= numberSystem) {
                throw createError("Wrong number");
            }

            if (number <= 36) {
                if (number < 10) {
                    return std::string(1, (char) ('0' + number));
                } else {
                    return std::string(1, (char) ('A' + (number - 10)));
                }
            } else {
                std::string result;
                while (number > 0) {
                    result = (char) ('0' + (number % 10)) + result;
                    number /= 10;
                }
                return "${" + result + "}";
            }
        }
    }
}