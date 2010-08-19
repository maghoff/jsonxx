#ifndef JSONXX_UTIL_HPP
#define JSONXX_UTIL_HPP

#include <ostream>
#include <string>

#include "declspec.hpp"

namespace jsonxx {

JSONXX_DECLSPEC void write_quoted_string(std::ostream&, const std::string&);

} // namespace jsonxx

#undef JSONXX_DECLSPEC

#endif // UTIL_HPP
