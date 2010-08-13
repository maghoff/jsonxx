#include "object_filter.hpp"

namespace jsonxx {

object_filter::object_filter() :
    target(0)
{
}

object_filter::object_filter(object_listener* target_) :
    target(target_)
{
}

void object_filter::key(const std::string& k) {
    target->key(k);
}

void object_filter::start_object() {
    target->start_object();
}

void object_filter::end_object() {
    target->end_object();
}

void object_filter::start_array() {
    target->start_array();
}

void object_filter::end_array() {
    target->end_array();
}

void object_filter::value(const std::string& v) { target->value(v); }
void object_filter::value(int v) { target->value(v); }
void object_filter::value(double v) { target->value(v); }
void object_filter::value(bool_type v) { target->value(v); }
void object_filter::value(null_type v) { target->value(v); }

} // namespace jsonxx
