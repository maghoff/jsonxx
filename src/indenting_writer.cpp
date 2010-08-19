#include <cassert>
#include "indenting_writer.hpp"
#include "util.hpp"

namespace jsonxx {

indenting_writer::indenting_writer(std::ostream& out_) :
    writer_base(out_),
    state(in_key_value_pair),
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

void indenting_writer::prepare_for_key() {
    if (state == other) out << ',';
    state = other;
    newline();
}

void indenting_writer::prepare_for_value() {
    if (state == in_key_value_pair) {
        // do nothing
    } else if (state == start_of_array) {
        newline();
    } else if (state == other) {
        out << ',';
        newline();
    }

    state = other;
}

void indenting_writer::key(const std::string& name) {
    writer_base::key(name);
    out << ' ';
    state = in_key_value_pair;
}

void indenting_writer::start_object() {
    writer_base::start_object();
    state = start_of_object;
    indentation_level++;
}

void indenting_writer::end_object() {
    indentation_level--;
    newline();
    writer_base::end_object();
}

void indenting_writer::start_array() {
    writer_base::start_array();
    state = start_of_array;
    indentation_level++;
}

void indenting_writer::end_array() {
    indentation_level--;
    newline();
    writer_base::end_array();
}

}
