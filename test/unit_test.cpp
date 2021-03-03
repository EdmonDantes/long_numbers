//
// Created by masa_ on 12.09.2020.
//

#include <iostream>
#include <sstream>
#include <fstream>
#include "../library/bit_stream_util.h"
#include "../library/big_integer.h"

using namespace long_number;
using namespace numsysconverter;

std::string getRandomString(int length) {
    std::stringstream ss = std::stringstream();
    for (int i = 0; i < length; i++) {
        ss << '0' + (rand() % 10);
    }
    return ss.str();
}

int main() {
    int startLength = 100;
    int lastLength = 40000;
    int h = 200;
    int warmCount = 10;

    std::string outputFile = "output.csv";

    auto file = std::ofstream(outputFile);
    file << "Length;It is warm;Nanoseconds for generation;Nanoseconds for transfers;Nanoseconds for operation;Number \"A\";Number\"B\";Number \"C\"\n";

    for (int i = startLength; i < lastLength; i += h) {
        for (int j = 0; j < warmCount; j++) {
            auto generationStart = getTime();
            auto aStr = getRandomString(i);
            auto bStr = getRandomString(i - 1);
            auto generationEnd = getTime();

            auto transferStart = getTime();
            auto a = BigInteger(aStr);
            auto b = BigInteger(bStr);
            auto transferEnd = getTime();


            auto start = getTime();
            auto c = a / b;
            auto end = getTime();

            file << i << ";False;" << getTimeNano(generationStart, generationEnd) << ';' << getTimeNano(transferStart, transferEnd) << ';' << getTimeNano(start, end) << ';' << a.toString() << ';' << b.toString() << ';' << c.toString() << '\n';
        }

        auto generationStart = getTime();
        auto aStr = getRandomString(i);
        auto bStr = getRandomString(i);
        auto generationEnd = getTime();

        auto transferStart = getTime();
        auto a = BigInteger(aStr);
        auto b = BigInteger(bStr);
        auto transferEnd = getTime();


        auto start = getTime();
        auto c = a / b;
        auto end = getTime();

        file << i << ";True;" << getTimeNano(generationStart, generationEnd) << ';' << getTimeNano(transferStart, transferEnd) << ';' << getTimeNano(start, end) << ';' << a.toString() << ';' << b.toString() << ';' << c.toString() << '\n';

        file.flush();

        std::cout << "Ended " << i << '\n';
    }

    file.close();
}