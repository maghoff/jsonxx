#include "compact_writer.hpp"

namespace jsonxx {

void write_quoted_string(std::ostream& out, const std::string& str) {
    // TODO: Escaping
    out << '"' << str << '"';
}


compact_writer::compact_writer(std::ostream& out_) :
    out(out_),
    skip_comma(true)
{
    out << std::boolalpha;
}

compact_writer::~compact_writer() {

}

void compact_writer::maybe_comma() {
    if (skip_comma) skip_comma = false;
    else out << ',';
}

void compact_writer::key(const std::string& name) {
    maybe_comma();
    write_quoted_string(out, name);
    out << ':';
}

void compact_writer::start_object() {
    out << '{';
    skip_comma = true;
}

void compact_writer::end_object() {
    out << '}';
}

void compact_writer::start_array() {
    maybe_comma();
    out << '[';
    skip_comma = true;
}

void compact_writer::end_array() {
    out << ']';
}

void compact_writer::value(const std::string& v) {
    write_quoted_string(out, v);
}

void compact_writer::value(int v) {
    out << v;
}

void compact_writer::value(double v) {
    out << v;
}

void compact_writer::value(bool_type v) {
    out << v.value;
}

void compact_writer::value(null_type) {
    out << "null";
}

}
