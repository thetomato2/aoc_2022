#ifndef TYPES_HH_
#define TYPES_HH_
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t b8;
typedef int32_t b32;

typedef float f32;
typedef double f64;

typedef size_t szt;
typedef unsigned char byt;
// using byt  u8; // TODO: is this different?

typedef wchar_t wchar;

typedef uintptr_t umm;
typedef intptr_t smm;

#define U8_MIN 0u
#define U8_MAX 0xffu
#define S8_MIN (-0x7f - 1)
#define S8_MAX 0x7f

#define U16_MIN 0u
#define U16_MAX 0xffffu
#define S16_MIN (-0x7fff - 1)
#define S16_MAX 0x7fff

#define U32_MIN 0u
#define U32_MAX 0xffffffffu
#define S32_MIN (-0x7fffffff - 1)
#define S32_MAX 0x7fffffff

#define U64_MIN 0ull
#define U64_MAX 0xffffffffffffffffull
#define S64_MIN (-0x7fffffffffffffffll - 1)
#define S64_MAX 0x7fffffffffffffffll

#define F32_MIN 1.175494351e-38f
#define F32_MAX 3.402823466e+38f
#define F64_MIN 2.2250738585072014e-308
#define F64_MAX 1.7976931348623158e+308

#define bit(x) (1 << x)

#define kilobytes(val) ((val)*1024)
#define megabytes(val) (kilobytes(val) * 1024)
#define gigabytes(val) (megabytes(val) * 1024)
#define terabytes(val) (gigabytes(val) * 1024)

#define function static
#define internal static
#define global   static
#define local    static

#ifdef _DEBUG
    #define assert(expr)            \
        if (!(expr)) {              \
            *(volatile int *)0 = 0; \
        }
#else
    #define assert(exp)
#endif

#define ctassert(Expr) static_assert(Expr, "Assertion failed: " #Expr)

#if _DEBUG
    #define NOT_IMPLEMENTED assert(!"Not Implemented")
#else
    #define NOT_IMPLEMENTED NotImplemented !!!!!!!!!!!!!!
#endif

#define INVALID_CODE_PATH assert(!"Invalid Code Path")
#define INVALID_DEFAULT_CASE \
    default: {               \
        INVALID_CODE_PATH;   \
    } break

#define count_of(array) (sizeof(array) / sizeof(array[0]))
#define offset_of(type, member) (umm)&(((type *)0)->member
#define member(T, m) (((T *)0)->m)

#define align_up_pow2(x, p)   (((x) + (p)-1) & ~((p)-1))
#define align_down_pow2(x, p) ((x) & ~((p)-1))

// #define min(a, b)      (((a) < (b)) ? (a) : (b))
// #define max(a, b)      (((a) > (b)) ? (a) : (b))
// #define clamp(a, x, b) (((x) < (a)) ? (a) : ((b) < (x)) ? (b) : (x))

#define stringify_(s) #s
#define stringify(s)  stringify_(s)
#define glue_(a, b)   a##b
#define glue(a, b)    glue_(a, b)

#if defined(_MSC_VER)
    #define threadvar __declspec(thread)
#elif defined(__clang__)
    #define threadvar __thread
#else
    #error threadvar defined for this compiler
#endif

#endif
