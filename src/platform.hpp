#ifndef JSONXX_PLATFORM_HPP
#define JSONXX_PLATFORM_HPP

#if defined(_MSC_VER)

// MSVC

#define JSONXX_IMPORT_DECLSPEC __declspec(dllimport)
#define JSONXX_EXPORT_DECLSPEC __declspec(dllexport)

#define JSONXX_DEPRECATED(msg) __declspec(deprecated(msg))


#elif defined(__clang__)

// CLANG

#define JSONXX_IMPORT_DECLSPEC
#define JSONXX_EXPORT_DECLSPEC

#define JSONXX_DEPRECATED(msg) __attribute__((deprecated(msg)))


#elif defined(__GNUC__)

// GCC

#define JSONXX_GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)


#define JSONXX_IMPORT_DECLSPEC
#define JSONXX_EXPORT_DECLSPEC

#if JSONXX_GCC_VERSION >= 40500
#define JSONXX_DEPRECATED(msg) __attribute__((deprecated(msg)))
#else
#define JSONXX_DEPRECATED(msg) __attribute__((deprecated))
#endif


#undef JSONXX_GCC_VERSION


#else

// None of the above

#define JSONXX_IMPORT_DECLSPEC
#define JSONXX_EXPORT_DECLSPEC

#define JSONXX_DEPRECATED(msg)

#endif


#endif // JSONXX_PLATFORM_HPP
