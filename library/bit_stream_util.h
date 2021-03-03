//
// Created by masa_ on 14.09.2020.
//

#ifndef LONG_NUMBERS_LIBRARY_BIT_STREAM_UTIL_H_
#define LONG_NUMBERS_LIBRARY_BIT_STREAM_UTIL_H_

#include "bit_stream.h"
#include "number_system_converter.h"

namespace long_number {
    namespace bit_stream_util {
        int8 compare(const BitStream* a, const BitStream* b, numsysconverter::NumberSystem operationNumberSystem);

        BitStream* sum(const BitStream* a, const BitStream* b, numsysconverter::NumberSystem operationNumberSystem);
        BitStream* sub(const BitStream* a, const BitStream* b, numsysconverter::NumberSystem operationNumberSystem);
        BitStream* mult(const BitStream* a, const BitStream* b, numsysconverter::NumberSystem operationNumberSystem);
        BitStream* mult(const BitStream* a, const uintmax b, numsysconverter::NumberSystem operationNumberSystem);
        BitStream** div(const BitStream* a, const BitStream* b, numsysconverter::NumberSystem operationNumberSystem);
        BitStream** div(const BitStream* a, const uintmax b, numsysconverter::NumberSystem operationNumberSystem);
    }
}
#endif //LONG_NUMBERS_LIBRARY_BIT_STREAM_UTIL_H_
