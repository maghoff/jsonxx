#include "parser2_state.hpp"
#include "object_listener.hpp"
#include "scanner_listener_stack.hpp"
#include "expect_comma_or_end_object.hpp"

namespace jsonxx {

expect_comma_or_end_object::expect_comma_or_end_object(parser2_state& s_) : error_fallback(", or }"), s(s_) { }

void expect_comma_or_end_object::end_object() {
	s.listener.end_object();
	s.stack.pop();
}

void expect_comma_or_end_object::comma() {
	s.stack.pop();
	s.stack.push(s.expect_key);
}

} // namespace jsonxx
