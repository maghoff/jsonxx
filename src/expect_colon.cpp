#include "parser2_state.hpp"
#include "scanner_listener_stack.hpp"
#include "expect_colon.hpp"

namespace jsonxx {

expect_colon::expect_colon(parser2_state& s_) : error_scanner_listener(":"), s(s_) { }

void expect_colon::colon() {
	s.stack.pop();
}

} // namespace jsonxx
