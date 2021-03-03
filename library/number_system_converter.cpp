//
// Created by masa_ on 12.09.2020.
//
#include <sstream>
#include <cmath>
#include "array_util.h"
#include "number_system_converter.h"

namespace long_number {
    namespace numsysconverter {

        NumberSystem::NumberSystem() noexcept = default;

        NumberSystem::NumberSystem(uintmax numberSystem) noexcept {
            this->numberSystem = numberSystem;
            double tmp = log2(numberSystem);
            this->countOfBits = (uintmax)tmp;
            this->countOfBits += (tmp - this->countOfBits > _MAX_PRECISION ? 1 : 0);
            this->isPowerOf2 =
                (((uintmax) 1) << this->countOfBits) == numberSystem;

            if (isPowerOf2) {
                this->numberForMod = this->numberSystem - 1;
            }

        }

        ConvertConfig::ConvertConfig(NumberSystem from, NumberSystem to, bool reverseInput) noexcept {
            this->from = from;
            this->to = to;
            this->reverseInput = reverseInput;
        }

        ConvertConfig ConvertConfig::setReverseInput(bool reverseInput) const noexcept {
            return { from, to, reverseInput };
        }

        ConvertConfig ConvertConfig::revertNumberSystem() const noexcept {
            return { to, from };
        }

        BitStream* convertTo(const BitStream& inputNumber, const ConvertConfig config) {
            BitStream originalNumber = inputNumber.copy();
            uintmax numberLength = originalNumber.getCount(config.from.countOfBits);
            auto resultNumber = new BitStream(numberLength, config.from.countOfBits);

            uintmax startIndexOriginal = 0;
            uintmax endIndexOriginal = numberLength;

            if (config.reverseInput) {
                while (endIndexOriginal >= startIndexOriginal) {
                    uintmax tmp = 0;
                    for (uintmax i = endIndexOriginal - 1; i < numberLength && (i >= startIndexOriginal || tmp != 0 && i >= 0); i--) {
                        tmp = tmp * config.from.numberSystem + originalNumber.get(i, config.from.countOfBits);
                        originalNumber.set(config.to.isPowerOf2 ? tmp >> config.to.countOfBits : tmp / config.to.numberSystem,
                            0,
                            config.from.countOfBits,
                            i,
                            config.from.countOfBits);
                        tmp = config.to.isPowerOf2 ? tmp & config.to.numberSystem : tmp % config.to.numberSystem;

                        if (i == endIndexOriginal - 1 && originalNumber.get(i, config.from.countOfBits) == 0) {
                            endIndexOriginal--;
                        }

                        if (i < startIndexOriginal) {
                            startIndexOriginal--;
                        }
                    }

                    while (originalNumber.get(startIndexOriginal,
                        config.from.countOfBits) == 0 && endIndexOriginal >= startIndexOriginal) {
                        startIndexOriginal++;
                    }

                    resultNumber->append(tmp, config.to.countOfBits);
                }
            } else {
                while (startIndexOriginal < endIndexOriginal) {
                    uintmax tmp = 0;
                    uintmax i = startIndexOriginal;
                    for (; i < endIndexOriginal || tmp != 0 && i < numberLength; i++) {
                        tmp = tmp * config.from.numberSystem + originalNumber.get(i, config.from.countOfBits);
                        originalNumber.set(config.to.isPowerOf2 ? tmp >> config.to.countOfBits : tmp / config.to.numberSystem,
                            0,
                            config.from.countOfBits,
                            i,
                            config.from.countOfBits);
                        tmp = config.to.isPowerOf2 ? tmp & config.to.numberForMod : tmp % config.to.numberSystem;

                        if (i == startIndexOriginal && originalNumber.get(i, config.from.countOfBits) == 0) {
                            startIndexOriginal++;
                        }

                        if (i >= endIndexOriginal) {
                            endIndexOriginal++;
                        }
                    }

                    while (originalNumber.get(endIndexOriginal - 1,
                        config.from.countOfBits) == 0 && endIndexOriginal > startIndexOriginal) {
                        endIndexOriginal--;
                    }

                    resultNumber->append(tmp, config.to.countOfBits);
                }
            }

            return resultNumber;
        }

        BitStream* convertTo(const std::string number, const ConvertConfig config) {
            BitStream* bs = convertTo(number.begin(), number.end(), number.length(), config);
            return bs;
        }

        BitStream* convertTo(const uintmax number, const ConvertConfig config) {
            BitStream* result = new BitStream(sizeof(number) * 8);
            result->append(number, true);
            return result;
        }

        const std::string convertToString(const BitStream& bs, const NumberSystem to, const bool reverseInput) {
            std::stringstream ss;

            uintmax bitSize = bs.getCountOfBits();

            if (reverseInput) {
                if (bitSize % to.countOfBits != 0) {
                    uint8 modCountOfBits = bitSize % to.countOfBits;
                    ss << convert_util::uIntToString(bs.get(bs.getCount(modCountOfBits) - 1, modCountOfBits), to.numberSystem);
                }

                for (uintmax i = bs.getCount(to.countOfBits) - 1; i < bs.getCount(to.countOfBits); i--) {
                    ss << convert_util::uIntToString(bs.get(i, to.countOfBits), to.numberSystem);
                }
            } else {
                for (uintmax i = 0; i < bs.getCount(to.countOfBits); i++) {
                    ss << convert_util::uIntToString(bs.get(i, to.countOfBits), to.numberSystem);
                }

                if (bitSize % to.countOfBits != 0) {
                    uint8 modCountOfBits = bitSize % to.countOfBits;
                    ss << convert_util::uIntToString(bs.get(bs.getCount(modCountOfBits) - 1, modCountOfBits), to.numberSystem);
                }
            }

            return ss.str();
        }

        BitStream* writeTo(std::string str, const NumberSystem numberSystem, bool reverseOutput) {
            return writeTo(str.begin(), str.end(), str.length(), numberSystem, reverseOutput);
        }


    }
}
