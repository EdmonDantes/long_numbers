//
// Created by masa_ on 16.09.2020.
//

#ifndef LONG_NUMBERS_LIBRARY_BIG_INTEGER_H_
#define LONG_NUMBERS_LIBRARY_BIG_INTEGER_H_

#include "common.h"
#include "bit_stream.h"
#include "number_system_converter.h"

namespace long_number {
    class BigInteger {
        #ifdef _ENABLE_DEBUG
     public:
        #else
     private:
        #endif
        static const long_number::numsysconverter::NumberSystem TO_NUMBER_SYSTEM;
        BitStream* intValue = nullptr;
        bool negative = false;

        BigInteger();
        BigInteger(BitStream* intValue, bool negative);


     public:

        static const BigInteger& ZERO;
        static const BigInteger& ONE;
        static const BigInteger& NEGATIVE_ONE;

        BigInteger(std::string number, long_number::numsysconverter::NumberSystem from);
        explicit BigInteger(std::string number);
        explicit BigInteger(intmax number);
        explicit BigInteger(const BigInteger* integer);
        BigInteger(BigInteger&& integer) noexcept ;
        BigInteger(const BigInteger& integer);

        BigInteger& operator=(BigInteger&& integer) noexcept ;

        bool operator>(const BigInteger& b);
        bool operator<(const BigInteger& b);
        bool operator>=(const BigInteger& b);
        bool operator<=(const BigInteger& b);
        bool operator==(const BigInteger& b);

        BigInteger operator+(const BigInteger& b);
        BigInteger operator-(const BigInteger& b);
        BigInteger operator*(const BigInteger& b);
        BigInteger operator/(const BigInteger& b);
        BigInteger operator%(const BigInteger& b);

        BigInteger* divAndMod(const BigInteger& b, BigInteger* array = nullptr);

        BigInteger* copy() const;

        ~BigInteger();

        const std::string toString() const;
    };


}

#endif //LONG_NUMBERS_LIBRARY_BIG_INTEGER_H_
