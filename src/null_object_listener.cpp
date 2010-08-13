#include "null_object_listener.hpp"

namespace jsonxx {

null_object_listener::null_object_listener() { }

void null_object_listener::key(const std::string&) { }

void null_object_listener::start_object() { }
void null_object_listener::end_object() { }

void null_object_listener::start_array() { }
void null_object_listener::end_array() { }

void null_object_listener::value(const std::string&) { }
void null_object_listener::value(int) { }
void null_object_listener::value(double) { }
void null_object_listener::value(bool_type) { }
void null_object_listener::value(null_type) { }

} // namespace jsonxx
