//
// Created by masa_ on 12.09.2020.
//
#ifndef LONG_NUMBERS__COMMON_H_
#define LONG_NUMBERS__COMMON_H_

#include <cstdint>
#include <chrono>
#include "exception.h"

namespace long_number {


    #ifndef _DISABLE_DEBUG
    #ifndef _ENABLE_DEBUG
    //#define _ENABLE_DEBUG
    #endif
    #endif

    #ifdef _WIN64
    #define _USING_X64
    #else
    #ifdef _WIN32
    #define _USING_X32
    #else
    #ifdef _WIN16
	#define _USING_X16
	#else

    #define _USING_X64
	//Replace line after
	//#error "Unsupport OS. Please add code for your OS"

	//Example code for your OS:

	// #ifdef _OS_X16
	// #define _USING_X16
	// #endif

	// #ifdef _OS_X32
	// #define _USING_X32
	// #endif

	// #ifdef _OS_X64
	// #define _USING_X64
	// #endif
	#endif


    #endif
    #endif

    //--------------For support systems------------\\
		//------- Types
    #ifndef _NOT_USE_LIBRARY_TYPES

    typedef uint8_t uint8;
    typedef int8_t int8;

    typedef uint16_t uint16;
    typedef int16_t int16;

    #ifdef _USING_X16
        typedef uint16_t uintmax;
		typedef int16_t intmax;
		#define _UINTMAX_MAX UINT16_MAX
		#define _INTMAX_MAX INT16_MAX
    #endif

    #ifdef _USING_X32
    typedef uint32_t uint32;
    typedef int32_t int32;

    typedef uint32_t uintmax;
    typedef int32_t intmax;

    #define _UINTMAX_MAX UINT32_MAX
    #define _INTMAX_MAX INT32_MAX
    #endif // _USING_X32

    #ifdef _USING_X64
    typedef uint32_t uint32;
		typedef int32_t int32;

		typedef uint64_t uint64;
		typedef int64_t int64;

		typedef uint64_t uintmax;
		typedef int64_t intmax;

		#define _UINTMAX_MAX UINT64_MAX
		#define _INTMAX_MAX INT64_MAX
    #endif // _USING_X64
    #endif

    #ifndef _MAX_PRECISION
    #define _MAX_PRECISION 0.000000001
    #endif // _MAX_PRECISION



    //------- Error block
    static inline exception createError(int code) {
        return long_number::exception("Long Numbers Lib: Error №" + std::to_string(code), code);
    }

    static inline exception createError(const std::string& message) {
        return long_number::exception("Long Numbers Lib: Message: " + message, -1);
    }

    static inline exception createError(int code, const std::string& message) {
        return long_number::exception("Long Numbers Lib: Error №" + std::to_string(code) + ".\nMessage: " + message, code);
    }

    #define _ERROR_CODE_WRONG_ARGUMENT 1

    //------ Time`s functions
    #define getTime() std::chrono::steady_clock::now()
    #define getTimeNano(s, e) std::chrono::duration_cast<std::chrono::nanoseconds>((e) - (s)).count()
    #define getTimeMilli(s, e) std::chrono::duration_cast<std::chrono::milliseconds>((e) - (s)).count()

    //------ Arithmetically functions
    template <typename T>
    static inline T min(T a, T b) {
        return a < b ? a : b;
    }

    template <typename T>
    static inline T max(T a, T b) {
        return a > b ? a : b;
    }
}

#endif //LONG_NUMBERS__COMMON_H_
