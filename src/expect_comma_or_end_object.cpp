#include "parser2_state.hpp"
#include "object_listener.hpp"
#include "scanner_listener_stack.hpp"
#include "expect_comma_or_end_object.hpp"

namespace jsonxx {

expect_comma_or_end_object::expect_comma_or_end_object(parser2_state& s_) :
	error_fallback(", or }"),
	s(s_),
	expect_colon_state(s_),
	expect_key_state(s_)
{
}

void expect_comma_or_end_object::end_object() {
	s.listener.end_object();
	s.stack.pop();
}

void expect_comma_or_end_object::comma() {
	s.stack.pop();
	s.stack.push(this);
	s.stack.push(s.expect_value);
	s.stack.push(&expect_colon_state);
	s.stack.push(&expect_key_state);
}

} // namespace jsonxx
