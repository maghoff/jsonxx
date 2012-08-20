#include <algorithm>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include "util.hpp"

namespace jsonxx {

bool is_string_special_char(char c) {
    return
        ((0 <= c) && (c < 0x20)) ||
        (c == '\\') ||
        (c == '\"')
    ;
}

encoding_error::encoding_error(const std::string& msg) :
    std::runtime_error(msg)
{
}

void write_quoted_string(std::ostream& out, const std::string& str) {
    out << '"';

    const char *begin = str.data(), *end = begin + str.size();

    for (const char* p = begin; p != end;) {
        const char* e = std::find_if(p, end, &is_string_special_char);
        out.write(p, e - p);
        p = e;
        if (p != end) {
            if (*p == '\"' || *p == '\\') out << '\\' << *p;
            else if (*p == '\b') out << "\\b";
            else if (*p == '\f') out << "\\f";
            else if (*p == '\n') out << "\\n";
            else if (*p == '\r') out << "\\r";
            else if (*p == '\t') out << "\\t";
            else {
                std::stringstream ss;
                ss <<
                    "jsonxx: It is impossible to encode the control character "
                    "0x" << std::setw(2) << std::setfill('0') << std::hex << (int)(unsigned char)(*p);
                throw encoding_error(ss.str());
            }
            ++p;
        }
    }

    out << '"';
}

} // namespace jsonxx
