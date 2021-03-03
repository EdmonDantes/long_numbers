//
// Created by masa_ on 12.09.2020.
//
#include <string.h>
#include "size_util.h"
#include "array_util.h"
#include "bit_stream.h"

namespace long_number {
    using namespace size_util;

    inline uintmax BitStream::catValue(uintmax value, uint8 offset, uint8 length, uint8 endShift) {
        if (length == 0 || offset == innerSize) {
            return 0;
        } else {
            return (((value >> offset) << (innerSize - length)) >> (innerSize - length)) << endShift;
        }
    }


    BitStream::BitStream() {
        value = new uintmax[size = 16];
        memset(value, 0, sizeof(uintmax) * 16);
    }

    BitStream::BitStream(uintmax countOfBits) {
        countOfBits = max(countOfBits, (uintmax) 16);
        size = size_util::divSize(countOfBits, innerSize) + (countOfBits % innerSize == 0 ? 0 : 1);
        value = new uintmax[size];
        memset(value, 0, multSize(size, sizeof(uintmax)));
    }

    BitStream::BitStream(const BitStream* bitStream) {
        this->value = bitStream->toArray();
        this->size = bitStream->size;
        this->index = bitStream->index;
        this->innerIndex = bitStream->innerIndex;

    }

    BitStream::BitStream(uintmax** values, const uintmax* sizes, uintmax count) {
        this->index = this->innerIndex = 0;
        this->size = 16;
        for (uintmax i = 0; i < count; i++) {
            this->size += sizes[i];
        }

        this->value = new uintmax[this->size];

        for (uintmax i = 0; i < count; i++) {
            memcpy(this->value + index, values[i], sizeof(uintmax) * sizes[i]);
            index += sizes[i];
        }
    }

    BitStream::BitStream(uintmax** values, uintmax size, uintmax count) {
        this->size = size * count + 16;
        this->value = new uintmax[this->size];

        for (uintmax i = 0; i < count; i++) {
            memcpy(this->value, values[i], sizeof(uintmax) * size);
        }

        this->index = this->innerIndex = 0;
    }

    BitStream::~BitStream() {
        delete[] value;
    }

    void BitStream::append(uintmax val, uint8 offset, uint8 length) {
        length = min(length, innerSize);
        offset = min(offset, innerSize);

        if (length > innerSize - innerIndex) {
            if (innerIndex < innerSize) {
                value[index] |= catValue(val, offset, length, innerIndex);
                length -= subSize(innerSize, innerIndex);
            }

            index++;
            innerIndex = 0;

            if (index >= size) {
                uintmax* tmp = array_util::addToEnd(value, size, size / 2 + 1);
                delete[] value;
                value = tmp;
                memset(tmp + size, 0, size / 2 * sizeof(*tmp));
                size += size / 2 + 1;
            }
        }
        value[index] |= catValue(val, offset, length, innerIndex);
        innerIndex += length;
    }

    void BitStream::append(uintmax val, uint8 offset, bool autoLength) {
        if (autoLength) {
            uint8 length = multSize(8, sizeof(val));
            uintmax checker = 1ull << (length - 1);
            while (length > 0) {
                if ((val & checker) == 0) {
                    length--;
                    checker = checker >> 1;
                } else {
                    break;
                }
            }

            if (length > 0) {
                this->append(val, offset, length);
            }
        } else {
            this->append(val, offset, (uint8)(sizeof(val) * 8));
        }
    }

    void BitStream::append(uintmax val, uint8 length) {
        this->append(val, 0, length);
    }

    void BitStream::append(uintmax val, bool autoLength) {
        this->append(val, 0, autoLength);
    }

    void BitStream::append(uintmax val) {
        this->append(val, true);
    }

    uintmax BitStream::get(uintmax index, uint8 countOfBit) const {
        if (countOfBit > innerSize) {
            throw createError(_ERROR_CODE_WRONG_ARGUMENT,
                "countOfBit is wrong. It must be less then " + std::to_string(innerSize));
        }

        uintmax startBit = index * countOfBit;
        uintmax indexValue = startBit / innerSize;
        uintmax innerOffset = startBit % innerSize;

        if (indexValue >= size) {
            throw createError(_ERROR_CODE_WRONG_ARGUMENT, "startBit is too much");
        }

        if (innerOffset + countOfBit <= innerSize) {
            return catValue(value[indexValue], innerOffset, countOfBit, 0);
        } else {
            uintmax result = value[indexValue] >> innerOffset;
            countOfBit -= (innerSize - innerOffset);
            indexValue++;


            result |= catValue(value[indexValue], 0, countOfBit, subSize(innerSize, innerOffset));

            return result;
        }

    }

    bool BitStream::have(uintmax index, uint8 countOfBit) const {
        if (countOfBit > innerSize) {
            throw createError(_ERROR_CODE_WRONG_ARGUMENT,
                "countOfBit is wrong. It must be less then " + std::to_string(innerSize));
        }

        uintmax startBit = index * countOfBit;

        return startBit / innerSize < this->index || startBit / innerSize == this->index && (startBit % innerSize) + countOfBit <= innerIndex;

    }

    void BitStream::set(uintmax val, uint8 offset, uint8 length, uintmax index, uint8 countOfBit) {

        if (countOfBit > innerSize) {
            throw createError(_ERROR_CODE_WRONG_ARGUMENT,
                "countOfBit is wrong. It must be less then " + std::to_string(innerSize));
        }

        uintmax startBit = index * countOfBit;
        uintmax indexValue = startBit / innerSize;
        uintmax innerOffset = startBit % innerSize;

        if (indexValue >= size) {
            throw createError(_ERROR_CODE_WRONG_ARGUMENT, "startBit is too much");
        }

        if (length > countOfBit) {
            throw createError(_ERROR_CODE_WRONG_ARGUMENT, "length must be more then countOfBit");
        }

        uintmax valSize = innerOffset + countOfBit;

        if (valSize <= innerSize) {
            value[indexValue] =
                catValue(value[indexValue], 0, innerOffset, 0) | catValue(val, offset, length, innerOffset) | catValue(
                    value[indexValue],
                    valSize,
                    innerSize,
                    valSize);
        } else {
            value[indexValue] =
                catValue(value[indexValue], 0, innerOffset, 0) | catValue(val, offset, length, innerOffset);
            value[indexValue + 1] = catValue(value[indexValue + 1],
                subSize(valSize, innerSize),
                innerSize,
                subSize(valSize, innerSize)) | catValue(val,
                offset + subSize(innerSize, innerOffset),
                length - subSize(valSize, innerSize),
                0);
        }
    }

    void BitStream::removeFromEnd(uintmax countOfBits) {
        if (countOfBits >= getCountOfBits()) {
            index = 0;
            innerIndex = 0;
            memset(value, 0, size);
        } else if (countOfBits > innerIndex) {
            value[index--] = 0;
            countOfBits = subSize(countOfBits, innerIndex);
            innerIndex = innerSize;
            int count = divSize(countOfBits, innerSize);
            countOfBits = subSize(countOfBits, multSize(count, innerSize));
            while (count > 0) {
                value[index--] = 0;
                count--;
            }

            if (countOfBits > 0) {
                removeFromEnd(countOfBits % innerSize);
            }
        } else {
            innerIndex -= countOfBits;
            uintmax tmp = 0;
            for (int i = 0; i < innerIndex; i++) {
                tmp += (1ull << i);
            }
            value[index] &= tmp;
        }
    }

    uintmax BitStream::getCountOfBits() const {
        return sizeof(size) * 8 * index + innerIndex;
    }

    uintmax BitStream::getCount(uint8 countOfBits) const {
        return getCountOfBits() / countOfBits;
    }

    uintmax* BitStream::getArray() {
        return value;
    }

    uintmax* BitStream::toArray() const {
        return array_util::resize(value, size, size);
    }

    BitStream* BitStream::reverse(uint8 length) const {
        auto tmp = new BitStream(this->getCountOfBits());
        for (intmax i = getCount(length) - 1; i > -1; i--) {
            tmp->append(this->get(i, length), length);
        }
        return tmp;
    }

    BitStream* BitStream::subStream(uintmax offset, uintmax length) const {
        auto result = new BitStream(length);

        uintmax arrayIndex = offset / innerSize;
        uintmax cellOffset = offset % innerSize;
        uintmax lastIndex = (offset + length) / innerSize;
        uintmax lastCellOffset = (offset + length) % innerSize;

        if (arrayIndex >= size) {
            throw createError(_ERROR_CODE_WRONG_ARGUMENT);
        }

        while (arrayIndex < lastIndex) {
            if (cellOffset > 0) {
                uint8 cellLength = innerSize - cellOffset;
                result->append(value[arrayIndex++], cellOffset, cellLength);
                cellOffset = 0;
            } else {
                result->append(value[arrayIndex++], innerSize);
            }
        }

        if (arrayIndex == lastIndex && arrayIndex < size && cellOffset < lastCellOffset) {
            uint8 cellLegth = lastCellOffset - cellOffset;
            result->append(value[arrayIndex], cellOffset, cellLegth);
        }

        return result;
    }

    uint8* BitStream::getBytes() const {
        uintmax resultSize = multSize(size, sizeof(uintmax));
        auto result = new uint8[resultSize];
        for (uintmax i = 0; i < getCount(8); i++) {
            result[i] = get(i, 8);
        }
        return result;
    }

    void BitStream::operator<<(uintmax val) {
        this->append(val);
    }

    BitStream::BitStream(BitStream&& stream) noexcept {
        operator=(std::move(stream));
    }

    BitStream::BitStream(const BitStream& stream) {
        operator=(stream);
    }

    BitStream& BitStream::operator=(const BitStream& stream) {
        this->size = stream.size;
        this->index = stream.index;
        this->innerIndex = stream.innerIndex;
        this->value = array_util::resize(stream.value, size, size);
        return *this;
    }

    BitStream& BitStream::operator=(BitStream&& stream) noexcept {
        this->size = stream.size;
        this->index = stream.index;
        this->innerIndex = stream.innerIndex;
        this->value = stream.value;

        stream.value = nullptr;
        stream.size = 0;
        stream.index = 0;
        stream.innerIndex = 0;
        return *this;
    }

    BitStream BitStream::copy() const {
        return std::move(BitStream(this));
    }

}