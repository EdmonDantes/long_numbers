//
// Created by masa_ on 12.09.2020.
//

#ifndef LONG_NUMBERS_LIBRARY_BIT_STREAM_H_
#define LONG_NUMBERS_LIBRARY_BIT_STREAM_H_

#include "common.h"

namespace long_number {
    class BitStream {
     private:
        static const uint8 innerSize = sizeof(uintmax) * 8;

        uintmax* value = nullptr;
        uintmax size = 0;
        uintmax index = 0;
        uint8 innerIndex = 0;

        static inline uintmax catValue(uintmax value, uint8 offset, uint8 length, uint8 endShift);
     public:
        BitStream();
        explicit BitStream(uintmax countOfBits);
        explicit BitStream(const BitStream* bitStream);
        BitStream(uintmax size, uintmax countOfBits) : BitStream(size * countOfBits) {};
        BitStream(uintmax** values, const uintmax* sizes, uintmax count);
        BitStream(uintmax** values, uintmax size, uintmax count);

        BitStream(BitStream&& stream) noexcept ;
        BitStream(const BitStream& stream);

        BitStream& operator=(BitStream&& stream) noexcept ;
        BitStream& operator=(const BitStream& stream);

        ~BitStream();

        void append(uintmax val, uint8 offset, uint8 length);
        void append(uintmax val, uint8 offset, bool autoLength);
        void append(uintmax val, uint8 length);
        void append(uintmax val, bool autoLength);
        void append(uintmax val);

        BitStream copy() const;

        uintmax get(uintmax index, uint8 countOfBit) const;
        bool have(uintmax index, uint8 countOfBit) const;

        void set(uintmax value, uint8 offset, uint8 length, uintmax index, uint8 countOfBit);

        void removeFromEnd(uintmax countOfBits);

        uintmax getCountOfBits() const;
        uintmax getCount(uint8 countOfBits) const;
        uint8* getBytes() const;

        uintmax* getArray();
        uintmax* toArray() const;

        BitStream* reverse(uint8 length) const;

        BitStream* subStream(uintmax offset, uintmax length) const;

        void operator << (uintmax val);
    };
}

#endif //LONG_NUMBERS_LIBRARY_BIT_STREAM_H_
