//
// Created by masa_ on 12.09.2020.
//

#ifndef LONG_NUMBERS_LIBRARY_CONVERT_UTIL_H_
#define LONG_NUMBERS_LIBRARY_CONVERT_UTIL_H_

#include "common.h"

namespace long_number {
    namespace convert_util {

        template <typename Iter>
        uintmax stringToUInt(Iter& begin, const Iter& end, const uintmax numberSystem) {
            uintmax result = 0;
            if (begin != end) {
                char ch = *(begin++);
                if (ch >= '0' && ch <= '9') {
                    result = ch - '0';
                } else if (ch >= 'A' && ch <= 'Z') {
                    result = ch - 'A' + 10;
                } else if (ch >= 'a' && ch <= 'z') {
                    result = ch - 'a' + 10;
                } else if (ch == '$' && begin != end && *begin == '{') {
                    begin++;
                    result = 0;
                    while (*begin >= '0' && *begin <= '9' && *begin != '}' && begin != end && result * 10 >= result) {
                        result *= 10;
                        result += *(begin++) - '0';
                    }
                    if (begin != end && *begin == '}') {
                        begin++;
                    }

                    if (result >= numberSystem) {
                        throw createError("Wrong number");
                    }
                } else {
                    throw createError("Wrong number format. Can not parse to number");
                }
            } else {
                throw createError("End of iteration");
            }

            return result;
        }

        std::string uIntToString(uintmax number, uintmax numberSystem);
    }
}

#endif //LONG_NUMBERS_LIBRARY_CONVERT_UTIL_H_
