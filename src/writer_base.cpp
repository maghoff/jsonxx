#include "util.hpp"
#include "writer_base.hpp"

namespace jsonxx {

writer_base::writer_base(std::ostream& out_) :
    out(out_)
{
}

writer_base::~writer_base() {
}

void writer_base::key(const std::string& name) {
    prepare_for_key();
    write_quoted_string(out, name);
    out << ':';
}

void writer_base::start_object() {
    prepare_for_value();
    out << '{';
}

void writer_base::end_object() {
    out << '}';
}

void writer_base::start_array() {
    prepare_for_value();
    out << '[';
}

void writer_base::end_array() {
    out << ']';
}

void writer_base::value(const std::string& v) {
    prepare_for_value();
    write_quoted_string(out, v);
}

void writer_base::value(int v) {
    prepare_for_value();
    out << v;
}

void writer_base::value(double v) {
    prepare_for_value();
    out << v;
}

void writer_base::value(bool_type v) {
    prepare_for_value();
    out << v.value;
}

void writer_base::value(null_type) {
    prepare_for_value();
    out << "null";
}

} // namespace jsonxx
