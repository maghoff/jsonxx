#include "parser2_state.hpp"
#include "object_listener.hpp"
#include "scanner_listener_stack.hpp"
#include "expect_key.hpp"

namespace jsonxx {

expect_key::expect_key(parser2_state& s_) : error_fallback("key"), s(s_) { }

void expect_key::string(const std::string& key) {
	s.listener.key(key);
	s.stack.pop();
	s.stack.push(s.expect_comma_or_end_object);
	s.stack.push(s.expect_value);
	s.stack.push(s.expect_colon);
}

} // namespace jsonxx
