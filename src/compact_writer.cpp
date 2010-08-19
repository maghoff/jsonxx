#include <cassert>
#include "compact_writer.hpp"
#include "util.hpp"

namespace jsonxx {

compact_writer::compact_writer(std::ostream& out_) :
    writer_base(out_),
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

void compact_writer::prepare_for_key() {
    comma_unless(skip_key_comma);
}

void compact_writer::prepare_for_value() {
    comma_unless(skip_array_comma);
}

void compact_writer::key(const std::string& name) {
    writer_base::key(name);
    state = skip_array_comma;
}

void compact_writer::start_object() {
    writer_base::start_object();
    state = skip_key_comma;
}

void compact_writer::start_array() {
    writer_base::start_array();
    state = skip_array_comma;
}

}
