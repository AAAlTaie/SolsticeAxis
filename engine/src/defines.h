#pragma once

// Unsigned int types
typedef unsigned char      u8;
typedef unsigned short     u16;
typedef unsigned int       u32;
typedef unsigned long long u64;

// Signed int types
typedef signed char        i8;
typedef signed short       i16;
typedef signed int         i32;
typedef signed long long   i64;

// Floating point types
typedef float              f32;
typedef double             f64;

// Boolean types
typedef int   b32;
typedef char  b8;

// Memory range types
typedef struct full_range64 {
    u64 offset;
    u64 size;
} full_range64;

typedef struct full_range32 {
    i32 offset;
    i32 size;
} full_range32;

// Static assertions
#if defined(__clang__) || defined(__gcc__)
    #define STATIC_ASSERT _Static_assert
#else
    #define STATIC_ASSERT static_assert
#endif

// Type size checks
STATIC_ASSERT(sizeof(u8)  == 1, "Expected u8 to be 1 byte.");
STATIC_ASSERT(sizeof(u16) == 2, "Expected u16 to be 2 bytes.");
STATIC_ASSERT(sizeof(u32) == 4, "Expected u32 to be 4 bytes.");
STATIC_ASSERT(sizeof(u64) == 8, "Expected u64 to be 8 bytes.");

STATIC_ASSERT(sizeof(i8)  == 1, "Expected i8 to be 1 byte.");
STATIC_ASSERT(sizeof(i16) == 2, "Expected i16 to be 2 bytes.");
STATIC_ASSERT(sizeof(i32) == 4, "Expected i32 to be 4 bytes.");
STATIC_ASSERT(sizeof(i64) == 8, "Expected i64 to be 8 bytes.");

STATIC_ASSERT(sizeof(f32) == 4, "Expected f32 to be 4 bytes.");
STATIC_ASSERT(sizeof(f64) == 8, "Expected f64 to be 8 bytes.");

// Boolean constants
#define true  1
#define false 0

// Invalid ID constants
#define INVALID_ID_U64 18446744073709551615UL
#define INVALID_ID     4294967295U
#define INVALID_ID_U32 INVALID_ID
#define INVALID_ID_U16 65535U
#define INVALID_ID_U8  255U

// Min/max constants
#define U64_MAX 18446744073709551615UL
#define U32_MAX 4294967295U
#define U16_MAX 65535U
#define U8_MAX  255U
#define U64_MIN 0UL
#define U32_MIN 0U
#define U16_MIN 0U
#define U8_MIN  0U

#define I8_MAX   127
#define I16_MAX  32767
#define I32_MAX  2147483647
#define I64_MAX  9223372036854775807LL
#define I8_MIN   (-I8_MAX - 1)
#define I16_MIN  (-I16_MAX - 1)
#define I32_MIN  (-I32_MAX - 1)
#define I64_MIN  (-I64_MAX - 1)

// Platform detection
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
    #define FULLPLATFORM_WINDOWS 1
    #ifndef _WIN64
        #error "64-bit is required on Windows!"
    #endif
#elif defined(__linux__) || defined(__gnu_linux__)
    #define FULLPLATFORM_LINUX 1
    #if defined(__ANDROID__)
        #define FULLPLATFORM_ANDROID 1
    #endif
#elif defined(__unix__)
    #define FULLPLATFORM_UNIX 1
#elif defined(_POSIX_VERSION)
    #define FULLPLATFORM_POSIX 1
#elif defined(__APPLE__)
    #define FULLPLATFORM_APPLE 1
    #include <TargetConditionals.h>
    #if TARGET_IPHONE_SIMULATOR
        #define FULLPLATFORM_IOS 1
        #define FULLPLATFORM_IOS_SIMULATOR 1
    #elif TARGET_OS_IPHONE
        #define FULLPLATFORM_IOS 1
    #elif TARGET_OS_MAC
        #define VK_USE_PLATFORM_MACOS_MVK
    #else
        #error "Unknown Apple platform"
    #endif
#else
    #error "Unknown platform!"
#endif

// API symbol visibility
#if defined(FULLPLATFORM_WINDOWS)
    #if defined(FULL_EXPORT)
        #define FULLAPI __declspec(dllexport)
    #else
        #define FULLAPI __declspec(dllimport)
    #endif
#elif defined(__clang__) || defined(__GNUC__)
    #if defined(FULL_EXPORT)
        #define FULLAPI __attribute__((visibility("default")))
    #else
        #define FULLAPI
    #endif
#else
    #define FULLAPI
#endif

// Build configuration
#if defined(_DEBUG) || defined(DEBUG)
    #define FULL_DEBUG 1
    #define FULL_RELEASE 0
#else
    #define FULL_DEBUG 0
    #define FULL_RELEASE 1
#endif

// Feature build flags
#if FULL_DEBUG
    #define FULL_HOT_RELOAD 1
#else
    #define FULL_HOT_RELOAD 0
#endif

// Math macros
#define FULL_CLAMP(value, min, max) (((value) <= (min)) ? (min) : ((value) >= (max)) ? (max) : (value))
#define FULL_MIN(x, y)              (((x) < (y)) ? (x) : (y))
#define FULL_MAX(x, y)              (((x) > (y)) ? (x) : (y))

// Inlining
#if defined(__clang__) || defined(__gcc__)
    #define FULL_INLINE    __attribute__((always_inline)) inline
    #define FULL_NOINLINE  __attribute__((noinline))
#elif defined(_MSC_VER)
    #define FULL_INLINE    __forceinline
    #define FULL_NOINLINE  __declspec(noinline)
#else
    #define FULL_INLINE    static inline
    #define FULL_NOINLINE
#endif

// Deprecation
#if defined(__clang__) || defined(__gcc__)
    #define FULL_DEPRECATED(message) __attribute__((deprecated(message)))
#elif defined(_MSC_VER)
    #define FULL_DEPRECATED(message) __declspec(deprecated(message))
#else
    #error "Unsupported compiler - don't know how to define deprecations!"
#endif

// Byte size macros
#define GIBIBYTES(amount) ((amount) * 1024ULL * 1024ULL * 1024ULL)
#define MEBIBYTES(amount) ((amount) * 1024ULL * 1024ULL)
#define KIBIBYTES(amount) ((amount) * 1024ULL)

#define GIGABYTES(amount) ((amount) * 1000ULL * 1000ULL * 1000ULL)
#define MEGABYTES(amount) ((amount) * 1000ULL * 1000ULL)
#define KILOBYTES(amount) ((amount) * 1000ULL)

// Alignment helpers
FULL_INLINE u64 full_get_aligned(u64 operand, u64 granularity) {
    return (operand + (granularity - 1)) & ~(granularity - 1);
}

FULL_INLINE full_range64 full_get_aligned_range(u64 offset, u64 size, u64 granularity) {
    u64 aligned_offset = full_get_aligned(offset, granularity);
    u64 aligned_end = full_get_aligned(offset + size, granularity);

    full_range64 result;
    result.offset = aligned_offset;
    result.size = aligned_end - aligned_offset;
    return result;
}

// Bit flags
#define FLAG_GET(flags, flag)        (((flags) & (flag)) == (flag))
#define FLAG_SET(flags, flag, on)    ((flags) = ((on) ? ((flags) | (flag)) : ((flags) & ~(flag))))
