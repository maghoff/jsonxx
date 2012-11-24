#include "parser2_state.hpp"
#include "object_listener.hpp"
#include "scanner_listener_stack.hpp"
#include "expect_start_array.hpp"

namespace jsonxx {

expect_start_array::expect_start_array(parser2_state& s_) :
	error_fallback("["),
	s(s_),
	expect_value_or_end_array_state(s_)
{
}

void expect_start_array::start_array() {
	s.listener.start_array();
	s.stack.pop();
	s.stack.push(&expect_value_or_end_array_state);
}

} // namespace jsonxx
