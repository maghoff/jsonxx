#include "parser2_state.hpp"
#include "object_listener.hpp"
#include "scanner_listener_stack.hpp"
#include "expect_key_or_end_object.hpp"

namespace jsonxx {

expect_key_or_end_object::expect_key_or_end_object(parser2_state& s_) :
	error_scanner_listener("key or }"),
	s(s_),
	expect_comma_or_end_object_state(s_),
	expect_colon_state(s_)
{
}

void expect_key_or_end_object::end_object() {
	s.listener.end_object();
	s.stack.pop();
}

void expect_key_or_end_object::string(const std::string& key) {
	s.listener.key(key);
	s.stack.pop();
	s.stack.push(&expect_comma_or_end_object_state);
	s.stack.push(s.expect_value);
	s.stack.push(&expect_colon_state);
}

} // namespace jsonxx
