#ifndef JSONXX_PLATFORM_HPP
#define JSONXX_PLATFORM_HPP

#ifdef WIN32

#define JSONXX_IMPORT_DECLSPEC __declspec(dllimport)
#define JSONXX_EXPORT_DECLSPEC __declspec(dllexport)

#else

#define JSONXX_IMPORT_DECLSPEC
#define JSONXX_EXPORT_DECLSPEC

#endif

#endif // JSONXX_PLATFORM_HPP
