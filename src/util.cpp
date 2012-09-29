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

namespace {

void write_code_point_in_2_octets(std::ostream& out, uint16_t point) {
    unsigned char a = 0xC0 | ((point >> 6) & 0x1F);
    unsigned char b = 0x80 | ((point >> 0) & 0x3F);
    out << a << b;
}

void write_code_point_in_3_octets(std::ostream& out, uint16_t point) {
    unsigned char a = 0xE0 | ((point >> 12) & 0x0F);
    unsigned char b = 0x80 | ((point >>  6) & 0x3F);
    unsigned char c = 0x80 | ((point >>  0) & 0x3F);
    out << a << b << c;
}

void write_code_point_in_4_octets(std::ostream& out, uint32_t point) {
    unsigned char a = 0xF0 | ((point >> 18) & 0x07);
    unsigned char b = 0x80 | ((point >> 12) & 0x3F);
    unsigned char c = 0x80 | ((point >>  6) & 0x3F);
    unsigned char d = 0x80 | ((point >>  0) & 0x3F);
    out << a << b << c << d;
}

}

void write_code_point_as_utf8(std::ostream& out, uint32_t point) {
    if (point <= 0x7f) {
        out << (char)(point & 0x7F);
    } else if (point <= 0x07FF) {
        write_code_point_in_2_octets(out, point);
    } else if (point <= 0xFFFF) {
        write_code_point_in_3_octets(out, point);
    } else if (point <= 0x0010FFFF) {
        write_code_point_in_4_octets(out, point);
    } else {
        throw encoding_error("Code point out of range (> 0x10FFFF)");
    }
}

} // namespace jsonxx
