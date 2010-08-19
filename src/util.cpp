#include "util.hpp"

namespace jsonxx {

void write_quoted_string(std::ostream& out, const std::string& str) {
    // TODO: Escaping
    out << '"' << str << '"';
}

} // namespace jsonxx
