#ifndef JSONXX_UTIL_HPP
#define JSONXX_UTIL_HPP

#include <ostream>
#include <stdexcept>
#include <string>

#include "declspec.hpp"

namespace jsonxx {

class JSONXX_DECLSPEC encoding_error : public std::runtime_error {
public:
    encoding_error(const std::string&);
};

JSONXX_DECLSPEC void write_quoted_string(std::ostream&, const std::string&);

} // namespace jsonxx

#undef JSONXX_DECLSPEC

#endif // UTIL_HPP
