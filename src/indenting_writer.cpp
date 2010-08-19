#include <cassert>
#include "indenting_writer.hpp"
#include "util.hpp"

namespace jsonxx {

indenting_writer::indenting_writer(std::ostream& out_) :
    out(out_),
    state(skip_array_comma),
    indentation_level(0)
{
    out << std::boolalpha;
}

indenting_writer::~indenting_writer() {

}

void indenting_writer::newline() {
    assert(indentation_level >= 0);

    static const char buf[] = "                ";
    const size_t bs = sizeof(buf) / sizeof(buf[0]) - 1; // Exclude \0-terminator

    size_t ind = 4 * static_cast<size_t>(indentation_level);

    out << '\n';
    for (size_t i=0; i<ind/bs; ++i) out.write(buf, bs);
    out.write(buf, ind % bs);
}

void indenting_writer::comma_unless(state_t s) {
    if (state == s) state = other;
    else out << ',';
}

void indenting_writer::maybe_key_comma() {
    comma_unless(skip_key_comma);
    newline();
}

void indenting_writer::maybe_array_comma() {
    comma_unless(skip_array_comma);
}

void indenting_writer::key(const std::string& name) {
    maybe_key_comma();
    write_quoted_string(out, name);
    out << ": ";
    state = skip_array_comma;
}

void indenting_writer::start_object() {
    maybe_array_comma();
    out << '{';
    state = skip_key_comma;
    indentation_level++;
}

void indenting_writer::end_object() {
    indentation_level--;
    newline();
    out << '}';
}

void indenting_writer::start_array() {
    maybe_array_comma();
    out << '[';
    state = skip_array_comma;
    indentation_level++;
}

void indenting_writer::end_array() {
    indentation_level--;
    newline();
    out << ']';
}

void indenting_writer::value(const std::string& v) {
    maybe_array_comma();
    write_quoted_string(out, v);
}

void indenting_writer::value(int v) {
    maybe_array_comma();
    out << v;
}

void indenting_writer::value(double v) {
    maybe_array_comma();
    out << v;
}

void indenting_writer::value(bool_type v) {
    maybe_array_comma();
    out << v.value;
}

void indenting_writer::value(null_type) {
    maybe_array_comma();
    out << "null";
}

}
