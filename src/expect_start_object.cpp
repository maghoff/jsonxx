#include "parser2_state.hpp"
#include "object_listener.hpp"
#include "scanner_listener_stack.hpp"
#include "expect_start_object.hpp"

namespace jsonxx {

expect_start_object::expect_start_object(parser2_state& s_) :
	error_scanner_listener("{"),
	s(s_),
	expect_key_or_end_object_state(s_)
{
}

void expect_start_object::start_object() {
	s.listener.start_object();
	s.stack.pop();
	s.stack.push(&expect_key_or_end_object_state);
}

} // namespace jsonxx
