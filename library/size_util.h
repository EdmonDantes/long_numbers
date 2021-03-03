//
// Created by masa_ on 12.09.2020.
//

#ifndef LONG_NUMBERS_LIBRARY_SIZE_UTIL_H_
#define LONG_NUMBERS_LIBRARY_SIZE_UTIL_H_

#include "common.h"

namespace long_number {
    namespace size_util {

        static inline uintmax addSize(uintmax a, uintmax b) {
            uintmax result = a + b;
            if (result < a || result < b) {
                throw createError(_ERROR_CODE_WRONG_ARGUMENT);
            } else {
                return result;
            }
        }

        static inline uintmax subSize(uintmax a, uintmax b) {
            uintmax result = a - b;
            if (result > a) {
                throw createError(_ERROR_CODE_WRONG_ARGUMENT);
            } else {
                return result;
            }
        }

        static inline uintmax multSize(uintmax a, uintmax b) {
            uintmax result = a * b;
            if (result < a || result < b) {
                throw createError(_ERROR_CODE_WRONG_ARGUMENT);
            } else {
                return result;
            }
        }

        static inline uintmax divSize(uintmax a, uintmax b) {
            uintmax result = a / b;
            if (result > a) {
                throw createError(_ERROR_CODE_WRONG_ARGUMENT);
            } else {
                return result;
            }
        }
    }
}

#endif //LONG_NUMBERS_LIBRARY_SIZE_UTIL_H_
