//
// Created by masa_ on 16.09.2020.
//
#include <algorithm>
#include <utility>
#include "big_integer.h"
#include "bit_stream_util.h"
namespace long_number {

    const numsysconverter::NumberSystem BigInteger::TO_NUMBER_SYSTEM = numsysconverter::NumberSystem(uintmax(1) << (sizeof(uintmax) * 4));
    const BigInteger& BigInteger::ZERO = BigInteger(numsysconverter::convertTo("0", numsysconverter::ConvertConfig(numsysconverter::NumberSystem(10), TO_NUMBER_SYSTEM)), false);
    const BigInteger& BigInteger::ONE = BigInteger(numsysconverter::convertTo("1", numsysconverter::ConvertConfig(numsysconverter::NumberSystem(10), TO_NUMBER_SYSTEM)), false);
    const BigInteger& BigInteger::NEGATIVE_ONE = BigInteger(numsysconverter::convertTo("1", numsysconverter::ConvertConfig(numsysconverter::NumberSystem(10), TO_NUMBER_SYSTEM)), true);

    BigInteger::BigInteger() {
        this->intValue = nullptr;
        this->negative = false;
    }

    BigInteger::BigInteger(BitStream* intValue, bool negative) {
        this->intValue = intValue;
        this->negative = negative;
    }

    BigInteger::BigInteger(std::string number, long_number::numsysconverter::NumberSystem from) {
        auto convertConfig = numsysconverter::ConvertConfig(from, TO_NUMBER_SYSTEM);

        uint8 startIndex = 0;
        if (number[0] == '-') {
            this->negative = true;
            startIndex++;
        } else {
            this->negative = false;
        }

        intmax integerIndex = number.find('.');
        if (integerIndex < 0) {
            integerIndex = number.find(',');
        }

        if (integerIndex > -1) {
            this->intValue = numsysconverter::convertTo(number.substr(startIndex, integerIndex - startIndex), convertConfig);
        } else {
            this->intValue = numsysconverter::convertTo(number.substr(startIndex, number.length() - startIndex), convertConfig);
        }

        if (bit_stream_util::compare(this->intValue, ZERO.intValue, TO_NUMBER_SYSTEM) == 0) {
            this->negative = false;
        }
    }

    BigInteger::BigInteger(std::string number) : BigInteger(std::move(number), numsysconverter::NumberSystem(10)) {}

    BigInteger::BigInteger(intmax number) : BigInteger(std::to_string(number), numsysconverter::NumberSystem(10)) {}

    BigInteger::BigInteger(const BigInteger* integer) {
        this->negative = integer->negative;
        this->intValue = new BitStream(integer->intValue);
    }

    BigInteger::BigInteger(BigInteger&& integer) noexcept {
        this->operator=(std::move(integer));
    }

    BigInteger::BigInteger(const BigInteger& integer) {
        this->negative = integer.negative;
        this->intValue = new BitStream(integer.intValue);

    }

    BigInteger& BigInteger::operator=(BigInteger&& integer) noexcept {
        this->negative = integer.negative;
        this->intValue = integer.intValue;
        integer.intValue = nullptr;
        return *this;
    }

    BigInteger BigInteger::operator+(const BigInteger& b) {
        if (this->intValue == nullptr || b.intValue == nullptr) {
            return std::move(BigInteger(nullptr, false));
        }

        if (this->negative == b.negative) {
            return BigInteger(bit_stream_util::sum(this->intValue, b.intValue, TO_NUMBER_SYSTEM), this->negative && b.negative);
        } else {
            int8 compareUnsigned = bit_stream_util::compare(this->intValue, b.intValue, TO_NUMBER_SYSTEM);

            BitStream* result;
            bool resultIsNegative = false;

            switch (compareUnsigned) {
                case 0:
                    result = new BitStream(ZERO.intValue);
                    resultIsNegative = false;
                    break;
                case -1:
                    result = bit_stream_util::sub(b.intValue, this->intValue, TO_NUMBER_SYSTEM);
                    resultIsNegative = !this->negative;
                    break;
                case 1:
                    result = bit_stream_util::sub(this->intValue, b.intValue, TO_NUMBER_SYSTEM);
                    resultIsNegative = this->negative;
                    break;
                default:
                    result = nullptr;
                    break;
            }

            return std::move(BigInteger(result, resultIsNegative));
        }
    }

    BigInteger BigInteger::operator-(const BigInteger& b) {

        if (this->intValue == nullptr || b.intValue == nullptr) {
            return std::move(BigInteger(nullptr, false));
        }

        BitStream* result = nullptr;
        bool resultIsNegative = false;

        int8 compareUnsigned = bit_stream_util::compare(this->intValue, b.intValue, TO_NUMBER_SYSTEM);

        if (this->negative == b.negative) {
            switch (compareUnsigned) {
                case 0:
                    result = new BitStream(ZERO.intValue);
                    resultIsNegative = false;
                    break;
                case -1:
                    result = bit_stream_util::sub(b.intValue, this->intValue, TO_NUMBER_SYSTEM);
                    resultIsNegative = !this->negative;
                    break;
                case 1:
                    result = bit_stream_util::sub(this->intValue, b.intValue, TO_NUMBER_SYSTEM);
                    resultIsNegative = this->negative;
                    break;
                default:
                    result = nullptr;
                    break;
            }
        } else {
            result = bit_stream_util::sum(this->intValue, b.intValue, TO_NUMBER_SYSTEM);
            resultIsNegative = this->negative;
        }

        return std::move(BigInteger(result, resultIsNegative));
    }

    BigInteger BigInteger::operator*(const BigInteger& b) {
        return std::move(BigInteger(bit_stream_util::mult(this->intValue, b.intValue, TO_NUMBER_SYSTEM), !(this->negative && b.negative) && (this->negative || b.negative)));
    }

    BigInteger BigInteger::operator/(const BigInteger& b) {
        auto array = new BigInteger[2];
        divAndMod(b, array);
        BigInteger result = std::move(array[0]);
        delete[] array;
        return std::move(result);
    }

    BigInteger BigInteger::operator%(const BigInteger& b) {
        auto array = new BigInteger[2];
        divAndMod(b, array);
        BigInteger result = std::move(array[1]);
        delete[] array;
        return std::move(result);
    }

    BigInteger* BigInteger::divAndMod(const BigInteger& b, BigInteger* array) {
        auto compareVal = bit_stream_util::compare(this->intValue, b.intValue, TO_NUMBER_SYSTEM);
        if (array == nullptr) {
            array = new BigInteger[2];
        }

        switch (compareVal) {
            case 0:
                array[0] = std::move(BigInteger(ZERO));
                array[1] = std::move(BigInteger(ZERO));
                break;
            case -1:
                array[0] = std::move(BigInteger(ZERO));
                array[1] = std::move(BigInteger(this));
                break;
            case 1:
                BitStream** streams = bit_stream_util::div(this->intValue, b.intValue, TO_NUMBER_SYSTEM);
                array[0] = std::move(BigInteger(streams[0], !(this->negative && b.negative) && (this->negative || b.negative)));
                array[1] = std::move(BigInteger(streams[1], false));
                break;
        }

        return array;
    }

    const std::string BigInteger::toString() const {
        if (this->intValue == nullptr) {
            return "NaN";
        }

        BitStream* tmp = numsysconverter::convertTo(*this->intValue, numsysconverter::ConvertConfig(TO_NUMBER_SYSTEM, numsysconverter::NumberSystem(10), true));
        const std::string result = numsysconverter::convertToString(*tmp, numsysconverter::NumberSystem(10), true);
        delete tmp;
        return (negative ? "-" : "") + result;
    }

    bool BigInteger::operator>(const BigInteger& b) {
        if (this->negative) {
            if (b.negative) {
                return bit_stream_util::compare(this->intValue, b.intValue, TO_NUMBER_SYSTEM) < 0;
            } else {
                return false;
            };
        } else {
            if (b.negative) {
                return true;
            } else {
                return bit_stream_util::compare(this->intValue, b.intValue, TO_NUMBER_SYSTEM) > 0;
            }
        }
    }

    bool BigInteger::operator<(const BigInteger& b) {
        if (this->negative) {
            if (b.negative) {
                return bit_stream_util::compare(this->intValue, b.intValue, TO_NUMBER_SYSTEM) > 0;
            } else {
                return true;
            }
        } else {
            if (b.negative) {
                return false;
            } else {
                return bit_stream_util::compare(this->intValue, b.intValue, TO_NUMBER_SYSTEM) < 0;
            }
        }
    }

    bool BigInteger::operator>=(const BigInteger& b) {
        return !operator<(b);
    }

    bool BigInteger::operator<=(const BigInteger& b) {
        return !operator>(b);
    }

    bool BigInteger::operator==(const BigInteger& b) {
        if (this->negative == b.negative) {
            return bit_stream_util::compare(this->intValue, b.intValue, TO_NUMBER_SYSTEM) == 0;
        }
        return false;
    }

    BigInteger* BigInteger::copy() const {
        return new BigInteger(this);
    }

    BigInteger::~BigInteger() {
        if (this->intValue != nullptr) {
            delete intValue;
        }
    }

}
