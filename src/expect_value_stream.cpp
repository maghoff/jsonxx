#include "parser2_state.hpp"
#include "scanner_listener_stack.hpp"
#include "expect_value_stream.hpp"

namespace jsonxx {

expect_value_stream::expect_value_stream(stack_parser_state& s_) :
	error_fallback("value"),
	s(s_),
	expect_value_state(s_)
{
}

void expect_value_stream::start_object() {
	s.stack.push(&expect_value_state);
	s.stack.start_object();
}

void expect_value_stream::start_array() {
	s.stack.push(&expect_value_state);
	s.stack.start_array();
}

void expect_value_stream::number(const std::string& value) {
	s.stack.push(&expect_value_state);
	s.stack.number(value);
}

void expect_value_stream::string(const std::string& value) {
	s.stack.push(&expect_value_state);
	s.stack.string(value);
}

void expect_value_stream::bool_true() {
	s.stack.push(&expect_value_state);
	s.stack.bool_true();
}

void expect_value_stream::bool_false() {
	s.stack.push(&expect_value_state);
	s.stack.bool_false();
}

void expect_value_stream::null() {
	s.stack.push(&expect_value_state);
	s.stack.null();
}

} // namespace jsonxx
