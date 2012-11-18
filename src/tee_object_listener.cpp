#include "tee_object_listener.hpp"

namespace jsonxx {

tee_object_listener::tee_object_listener(jsonxx::object_listener& a_, jsonxx::object_listener& b_) :
    a(a_), b(b_)
{
}

tee_object_listener::~tee_object_listener() {}

void tee_object_listener::key(const std::string& v) {
    a.key(v);
    b.key(v);
}

void tee_object_listener::start_object() {
    a.start_object();
    b.start_object();
}

void tee_object_listener::end_object() {
    a.end_object();
    b.end_object();
}

void tee_object_listener::start_array() {
    a.start_array();
    b.start_array();
}

void tee_object_listener::end_array() {
    a.end_array();
    b.end_array();
}

void tee_object_listener::value(const std::string& v) {
    a.value(v);
    b.value(v);
}

void tee_object_listener::value(int v) {
    a.value(v);
    b.value(v);
}

void tee_object_listener::value(double v) {
    a.value(v);
    b.value(v);
}

void tee_object_listener::value(bool_type v) {
    a.value(v);
    b.value(v);
}

void tee_object_listener::value(null_type v) {
    a.value(v);
    b.value(v);
}


} // namespace jsonxx
