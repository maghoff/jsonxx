#include <cassert>
#include "compact_writer.hpp"

namespace jsonxx {

void write_quoted_string(std::ostream& out, const std::string& str) {
    // TODO: Escaping
    out << '"' << str << '"';
}


compact_writer::compact_writer(std::ostream& out_) :
    out(out_),
    state(skip_array_comma)
{
    out << std::boolalpha;
}

compact_writer::~compact_writer() {

}

void compact_writer::comma_unless(state_t s) {
    if (state == s) state = other;
    else out << ',';
}

void compact_writer::maybe_key_comma() {
    comma_unless(skip_key_comma);
}

void compact_writer::maybe_array_comma() {
    comma_unless(skip_array_comma);
}

void compact_writer::key(const std::string& name) {
    maybe_key_comma();
    write_quoted_string(out, name);
    out << ':';
    state = skip_array_comma;
}

void compact_writer::start_object() {
    maybe_array_comma();
    out << '{';
    state = skip_key_comma;
}

void compact_writer::end_object() {
    out << '}';
}

void compact_writer::start_array() {
    maybe_array_comma();
    out << '[';
    state = skip_array_comma;
}

void compact_writer::end_array() {
    out << ']';
}

void compact_writer::value(const std::string& v) {
    maybe_array_comma();
    write_quoted_string(out, v);
}

void compact_writer::value(int v) {
    maybe_array_comma();
    out << v;
}

void compact_writer::value(double v) {
    maybe_array_comma();
    out << v;
}

void compact_writer::value(bool_type v) {
    maybe_array_comma();
    out << v.value;
}

void compact_writer::value(null_type) {
    maybe_array_comma();
    out << "null";
}

}
