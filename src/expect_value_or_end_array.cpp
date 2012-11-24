#include "parser2_state.hpp"
#include "object_listener.hpp"
#include "scanner_listener_stack.hpp"
#include "expect_value_or_end_array.hpp"

namespace jsonxx {

expect_value_or_end_array::expect_value_or_end_array(parser2_state& s_) :
	error_fallback("value or ]"),
	s(s_),
	expect_comma_or_end_array_state(s_)
{
}

void expect_value_or_end_array::end_array() {
	s.listener.end_array();
	s.stack.pop();
}

void expect_value_or_end_array::start_object() { fall_back_to_value(); s.stack.start_object(); }
void expect_value_or_end_array::start_array() { fall_back_to_value(); s.stack.start_array(); }
void expect_value_or_end_array::number(const std::string& v) { fall_back_to_value(); s.stack.number(v); }
void expect_value_or_end_array::string(const std::string& v) { fall_back_to_value(); s.stack.string(v); }
void expect_value_or_end_array::bool_true() { fall_back_to_value(); s.stack.bool_true(); }
void expect_value_or_end_array::bool_false() { fall_back_to_value(); s.stack.bool_false(); }
void expect_value_or_end_array::null() { fall_back_to_value(); s.stack.null(); }

void expect_value_or_end_array::fall_back_to_value() {
	s.stack.pop();
	s.stack.push(&expect_comma_or_end_array_state);
	s.stack.push(s.expect_value);
}

} // namespace jsonxx
