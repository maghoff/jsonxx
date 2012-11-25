#include <sstream>
#include "parser2_state.hpp"
#include "object_listener.hpp"
#include "scanner_listener_stack.hpp"
#include "expect_value.hpp"

namespace jsonxx {

expect_value::expect_value(parser2_state& s_) :
	error_fallback("value"),
	s(s_),
	expect_start_object_state(s_),
	expect_start_array_state(s_)
{
}

void expect_value::start_object() {
	s.stack.pop();
	s.stack.push(&expect_start_object_state);
	s.stack.start_object();
}

void expect_value::start_array() {
	s.stack.pop();
	s.stack.push(&expect_start_array_state);
	s.stack.start_array();
}

void expect_value::number(const std::string& value) {
	std::stringstream ss(value);
	double d;
	ss >> d;
	s.listener.value(d);
	s.stack.pop();
}

void expect_value::string(const std::string& value) {
	s.listener.value(value);
	s.stack.pop();
}

void expect_value::bool_true() {
	s.listener.value_bool(true);
	s.stack.pop();
}

void expect_value::bool_false() {
	s.listener.value_bool(false);
	s.stack.pop();
}

void expect_value::null() {
	s.listener.value_null();
	s.stack.pop();
}

} // namespace jsonxx
