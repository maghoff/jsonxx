#include "parser2_state.hpp"
#include "object_listener.hpp"
#include "scanner_listener_stack.hpp"
#include "expect_comma_or_end_array.hpp"

namespace jsonxx {

expect_comma_or_end_array::expect_comma_or_end_array(parser2_state& s_) : error_fallback(", or ]"), s(s_) { }

void expect_comma_or_end_array::end_array() {
	s.listener.end_array();
	s.stack.pop();
}

void expect_comma_or_end_array::comma() {
	s.stack.pop();
	s.stack.push(this);
	s.stack.push(s.expect_value);
}

} // namespace jsonxx
