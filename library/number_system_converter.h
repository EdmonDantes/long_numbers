//
// Created by masa_ on 12.09.2020.
//

#ifndef LONG_NUMBERS_LIBRARY_NUMBER_SYSTEM_CONVERTER_H_
#define LONG_NUMBERS_LIBRARY_NUMBER_SYSTEM_CONVERTER_H_

#include <string>
#include "common.h"
#include "bit_stream.h"
#include "convert_util.h"

namespace long_number {
    namespace numsysconverter {

        struct NumberSystem {
            uintmax numberSystem = 0;
            uint8 countOfBits = 0;

            uintmax numberForMod = 0;
            bool isPowerOf2 = false;

            NumberSystem() noexcept;
            explicit NumberSystem(uintmax numberSystem) noexcept;
        };

        struct ConvertConfig {
            NumberSystem from;
            NumberSystem to;

            bool reverseInput = false;

            ConvertConfig(NumberSystem from, NumberSystem to, bool reverseInput = false) noexcept;
            ConvertConfig(uintmax from, uintmax to, bool reverseInput = false) noexcept : ConvertConfig(NumberSystem(from), NumberSystem(to), reverseInput) {}

            inline ConvertConfig setReverseInput(bool reverseInput) const noexcept;
            inline ConvertConfig revertNumberSystem() const noexcept;
        };

        /*
            Method for convert number from one number system to other
            Convert number with size = *N* from *A* number system to *B* number system
            O(N^2 * log2(A)) +- 1%

            Max number system = 2^SystemBits = Machine word
            x86 = 2^32
            x64 = 2^64
        */
        BitStream* convertTo(const BitStream& number, const ConvertConfig config);

        template<typename Iter>
        BitStream* convertTo(Iter begin, const Iter end, const uintmax length, const ConvertConfig config) {
            BitStream number = BitStream(length, config.from.countOfBits);
            for (int i = 0; i < length && begin != end; i++)
            {
                number.append(convert_util::stringToUInt(begin, end, config.from.numberSystem), config.from.countOfBits);
            }
            return convertTo(number, config);
        }

        template<typename Iter>
        BitStream* convertTo(Iter begin, const Iter end, const ConvertConfig config) {
            return convertTo(begin, end, 16, config);
        }

        BitStream* convertTo(const std::string number, const ConvertConfig config);
        BitStream* convertTo(const uintmax number, const ConvertConfig config);

        const std::string convertToString(const BitStream& stream, const NumberSystem numberSystem, const bool reverseInput);

        template<typename Iter>
        BitStream* writeTo(Iter begin, const Iter end, const uintmax length, const NumberSystem numberSystem, bool reverseOutput = false) {
            auto number = new BitStream(length, numberSystem.countOfBits);
            for (int i = 0; i < length && begin != end; i++)
            {
                number->append(convert_util::stringToUInt(begin, end, numberSystem.numberSystem), numberSystem.countOfBits);
            }

            if (reverseOutput) {
                auto tmp = number->reverse(numberSystem.countOfBits);
                delete number;
                return tmp;
            } else {
                return number;
            }
        }

        BitStream* writeTo(std::string, const NumberSystem numberSystem, bool reverseOutput = false);
    }
}

#endif //LONG_NUMBERS_LIBRARY_NUMBER_SYSTEM_CONVERTER_H_
