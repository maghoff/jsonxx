#include "object_listener_stack.hpp"

namespace jsonxx {

object_listener_stack::object_listener_stack() {}

object_listener_stack::~object_listener_stack() {}

void object_listener_stack::push(object_listener* listener) { stack.push(listener); }
object_listener* object_listener_stack::top() { return stack.top(); }
void object_listener_stack::pop() { stack.pop(); }

void object_listener_stack::key(const std::string& key) { stack.top()->key(key); }

void object_listener_stack::start_object() { stack.top()->start_object(); }
void object_listener_stack::end_object() { stack.top()->end_object(); }

void object_listener_stack::start_array() { stack.top()->start_array(); }
void object_listener_stack::end_array() { stack.top()->end_array(); }

void object_listener_stack::value(const std::string& v) { stack.top()->value(v); }
void object_listener_stack::value(int v) { stack.top()->value(v); }
void object_listener_stack::value(double v) { stack.top()->value(v); }
void object_listener_stack::value(bool_type v) { stack.top()->value(v); }
void object_listener_stack::value(null_type v) { stack.top()->value(v); }

} // namespace jsonxx
