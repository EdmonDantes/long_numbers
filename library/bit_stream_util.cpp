//
// Created by masa_ on 14.09.2020.
//
#include "bit_stream_util.h"
#include <iostream>


namespace long_number {
    namespace bit_stream_util {
        int8 compare(const BitStream* a, const BitStream* b, numsysconverter::NumberSystem operationNumberSystem){
            if (a == nullptr || b == nullptr) {
                return 0;
            }

            uintmax aSize = a->getCount(operationNumberSystem.countOfBits);
            uintmax bSize = b->getCount(operationNumberSystem.countOfBits);
            if (aSize == bSize) {
                for (uintmax i = aSize - 1; i < aSize; i--) {
                    uintmax aCell = a->get(i, operationNumberSystem.countOfBits);
                    uintmax bCell = b->get(i, operationNumberSystem.countOfBits);
                    if (aCell != bCell) {
                        return aCell < bCell ? -1 : 1;
                    }
                }
                return 0;
            } else {
                return aSize < bSize ? -1 : 1;
            }
        }

        BitStream* sum(const BitStream* a, const BitStream* b, numsysconverter::NumberSystem operationNumberSystem) {
            if (a == nullptr || b == nullptr) {
                return nullptr;
            }

            auto countOfBits = operationNumberSystem.countOfBits;
            auto result = new BitStream(a->getCountOfBits() + b->getCountOfBits() + countOfBits);

            if (a->getCountOfBits() < b->getCountOfBits()) {
                std::swap(a, b);
            }

            uintmax nextTmp = 0;
            for (uintmax i = 0; i < b->getCount(countOfBits); i++) {
                uintmax tmp = nextTmp + b->get(i, countOfBits) + a->get(i, countOfBits);
                if (tmp >= operationNumberSystem.numberSystem) {
                    nextTmp = 1;
                    tmp = tmp % operationNumberSystem.numberSystem;
                } else {
                    nextTmp = 0;
                }
                result->append(tmp, countOfBits);
            }

            for (uintmax i = b->getCount(countOfBits); i < a->getCount(countOfBits); i++) {
                uintmax tmp = nextTmp + a->get(i, countOfBits);
                if (tmp >= operationNumberSystem.numberSystem) {
                    nextTmp = 1;
                    tmp = tmp % operationNumberSystem.numberSystem;
                } else {
                    nextTmp = 0;
                }
                result->append(tmp, countOfBits);
            }

            if (nextTmp > 0) {
                result->append(nextTmp, countOfBits);
            }

            return result;
        }

        BitStream* sub(const BitStream* a, const BitStream* b, numsysconverter::NumberSystem operationNumberSystem) {
            if (a == nullptr || b == nullptr) {
                return nullptr;
            }

            if (a->getCountOfBits() < b->getCountOfBits()) {
                return nullptr;
            }

            auto countOfBits = operationNumberSystem.countOfBits;
            auto result = new BitStream(max(a->getCountOfBits(), b->getCountOfBits()));

            uintmax nextTmp = 0;
            for (uintmax i = 0; i < b->getCount(countOfBits); i++) {
                uintmax aCell = a->get(i, countOfBits);
                uintmax tmp = aCell - b->get(i, countOfBits) - nextTmp;
                nextTmp = 0;

                if (tmp > aCell) {
                    nextTmp++;
                    tmp = operationNumberSystem.numberSystem - (_UINTMAX_MAX - tmp + 1);
                }

                result->append(tmp, countOfBits);
            }

            for (uintmax i = b->getCount(countOfBits); i < a->getCount(countOfBits); i++) {
                uintmax aCell = a->get(i, countOfBits);
                uintmax tmp = aCell - nextTmp;
                nextTmp = 0;

                while (tmp > aCell) {
                    nextTmp++;
                    tmp += operationNumberSystem.numberSystem;
                }

                result->append(tmp, countOfBits);
            }

            if (nextTmp > 0) {
                delete result;
                return nullptr;
            }

            return result;
        }

        BitStream* mult(const BitStream* a, const BitStream* b, numsysconverter::NumberSystem operationNumberSystem) {
            if (a == nullptr || b == nullptr) {
                return nullptr;
            }

            if (a->getCountOfBits() < b->getCountOfBits()) {
                std::swap(a, b);
            }

            uint8 bits = operationNumberSystem.countOfBits;
            BitStream* result = new BitStream(a->getCount(bits) + b->getCount(bits), bits);

            uintmax tmp = 0;
            for (uintmax i = 0, j; i < a->getCount(bits); i++) {
                for (j = 0; j < b->getCount(bits); j++) {
                    tmp += a->get(i, bits) * b->get(j, bits);
                    while (!result->have(i + j, bits)) {
                        result->append(0, bits);
                    }

                    tmp += result->get(i + j, bits);
                    result->set(tmp % operationNumberSystem.numberSystem, 0, bits, i + j, bits);

                    tmp /= operationNumberSystem.numberSystem;
                }

                while (tmp > 0) {
                    while (!result->have(i + j, bits)) {
                        result->append(0, bits);
                    }

                    tmp += result->get(i + j, bits);
                    result->set(tmp % operationNumberSystem.numberSystem, 0, bits, i + (j++), bits);
                    tmp /= operationNumberSystem.numberSystem;
                }
            }

            return result;
        }

        BitStream* mult(const BitStream* a, const uintmax b, numsysconverter::NumberSystem operationNumberSystem) {
            if (a == nullptr || b == 0) {
                return nullptr;
            }

            if (b == 1){
                return new BitStream(a);
            }

            uint8 bits = operationNumberSystem.countOfBits;
            BitStream* result = new BitStream(a->getCount(bits) + 1, bits);

            uintmax tmp = 0;
            for (uintmax i = 0; i < a->getCount(bits); i++) {
                tmp += a->get(i, bits) * b;
                result->append(tmp % operationNumberSystem.numberSystem, bits);
                tmp /= operationNumberSystem.numberSystem;
            }
            while (tmp != 0) {
                result->append(tmp % operationNumberSystem.numberSystem, bits);
                tmp /= operationNumberSystem.numberSystem;
            }

            return result;
        }

        BitStream** div(const BitStream* a, const BitStream* b, numsysconverter::NumberSystem operationNumberSystem) {

            BitStream** result = new BitStream*[2];
            const uint8 bits = operationNumberSystem.countOfBits;

            if (a->getCountOfBits() < b->getCountOfBits()) {
                result[0] = new BitStream(bits);
                result[0]->append(0, bits);
                result[1] = new BitStream(a);
                return result;
            }

            if (a->getCount(bits) == 0) {
                result[0] = new BitStream(bits);
                result[0]->append(0, bits);
                result[1] = new BitStream(bits);
                result[1]->append(0, bits);
                return result;
            }

            if (b->getCount(bits) == 0) {
                result[0] = new BitStream((uintmax) 0);
                result[1] = new BitStream((uintmax) 0);
                return result;
            }

            if (b->getCount(bits) == 1) {
                delete[] result;
                return div(a, b->get(0, bits), operationNumberSystem);
            }

            BitStream number = BitStream(max(a->getCount(bits), b->getCount(bits)), bits);

            uintmax d = operationNumberSystem.numberSystem / (b->get(b->getCount(bits) - 1, bits) + 1);

            auto tmpA = a->copy();
            auto _a = mult(&tmpA, d, operationNumberSystem);

            auto tmpB = b->copy();
            auto _b = mult(&tmpB, d, operationNumberSystem);

            uintmax n = _b->getCount(bits);
            uintmax m = max(_a->getCount(bits), a->getCount(bits) + 1) - n - 1;
            uintmax j = m;

            uintmax tmp = 0;
            uintmax _q;

            do {
                auto elementB = _b->get(n - 1, bits);
                _q = ((_a->have(n + j, bits) ? _a->get(n + j, bits) : 0) * operationNumberSystem.numberSystem + (_a->have(n + j - 1, bits) ? _a->get(n + j - 1, bits) : 0));
                auto r = _q % elementB;
                _q /= elementB;

                do {
                    if (_q == operationNumberSystem.numberSystem || _q * _b->get(n - 2, bits) > operationNumberSystem.numberSystem * r + (_a->have(n + j - 2, bits) ? _a->get(n + j - 2, bits) : 0)) {
                        _q--;
                        r += _b->get(n - 1, bits);
                    } else {
                        break;
                    }
                } while (r < operationNumberSystem.numberSystem);

                for (uintmax i = j; i <= j + n; i++) {
                    auto element = _a->have(i, bits) ? _a->get(i, bits) : 0;
                    auto newElement = element - _q * (_b->have(i - j, bits) ? _b->get(i - j, bits) : 0) - tmp;
                    tmp = 0;

                    if (newElement > operationNumberSystem.numberSystem) {
                        tmp = (_UINTMAX_MAX - newElement) / operationNumberSystem.numberSystem;
                        newElement += operationNumberSystem.numberSystem * tmp;

                        while (newElement > operationNumberSystem.numberSystem) {
                            tmp++;
                            newElement += operationNumberSystem.numberSystem;
                        }
                    }



                    _a->set(newElement, 0, bits, i, bits);
                }

                auto qj = _q;

                if (tmp != 0) {
                    tmp = 0;
                    qj--;

                    for (uintmax i = j; i <= j + n; i++) {
                        tmp += (_a->have(i, bits) ? _a->get(i, bits) : 0) + (_b->have(i - j, bits) ? _b->get(i - j, bits) : 0);
                        _a->set(tmp % operationNumberSystem.numberSystem, 0, bits, i, bits);
                        tmp /= operationNumberSystem.numberSystem;
                    }
                }



                j--;
                number.append(qj, bits);
            } while (j <= m);

            if (d == 1) {
                result[1] = _a;
            } else {
                auto modResult = div(_a, d, operationNumberSystem);
                if (modResult[1] != nullptr) {
                    delete modResult[1];
                    modResult[1] = nullptr;
                }
                result[1] = modResult[0];
                delete[] modResult;
                delete _a;
            }

            delete _b;


            result[0] = number.reverse(bits);

            uintmax l = result[0]->getCount(bits);
            for (uintmax i = l - 1; i < l; i--) {
                if (result[0]->get(i, bits) == 0) {
                    result[0]->removeFromEnd(bits);
                } else {
                    break;
                }
            }



            return result;
        }

        BitStream** div(const BitStream* a, const uintmax b, numsysconverter::NumberSystem operationNumberSystem) {

            BitStream** result = new BitStream* [2];
            result[0] = nullptr;
            result[1] = nullptr;
            if (a == nullptr) {
                return result;
            }

            uint8 bits = operationNumberSystem.countOfBits;

            if (a->getCount(bits) < 1) {
                return result;
            }

            BitStream* originalNumber = new BitStream(a);
            uintmax numberLength = originalNumber->getCount(bits);
            result[0] = originalNumber;

            uintmax tmp = 0;
            for (uintmax i = numberLength - 1; i < numberLength; i--) {
                tmp = tmp * operationNumberSystem.numberSystem + originalNumber->get(i, bits);
                originalNumber->set(tmp / b, 0, bits, i, bits);
                tmp %= b;
            }

            if (tmp != 0) {
                result[1] = new BitStream(bits);
                result[1]->append(tmp % operationNumberSystem.numberSystem, bits);
            }

            return result;
        }

    }
}
