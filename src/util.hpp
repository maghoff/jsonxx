#ifndef JSONXX_UTIL_HPP
#define JSONXX_UTIL_HPP

#include <ostream>
#include <stdexcept>
#include <string>

#include "declspec.hpp"

#ifdef _MSC_VER
#pragma warning(push)
// C4275: Warning about std::runtime_error not being dll-exported.
#pragma warning(disable: 4275)
#endif

namespace jsonxx {

class JSONXX_DECLSPEC encoding_error : public std::runtime_error {
public:
    encoding_error(const std::string&);
};

// Writes the given string to the given ostream. Characters that should be
// escaped in JSON strings are escaped. Quotes are added to the beginning and
// end. If the given string is not representable in JSON, an encoding_error
// exception is thrown.
JSONXX_DECLSPEC void write_quoted_string(std::ostream&, const std::string&);

} // namespace jsonxx

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#undef JSONXX_DECLSPEC

#endif // UTIL_HPP
