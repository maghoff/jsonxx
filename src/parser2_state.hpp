#ifndef JSONXX_PARSER2_STATE_HPP
#define JSONXX_PARSER2_STATE_HPP

#include "declspec.hpp"

namespace jsonxx {

class scanner_listener_stack;
class object_listener;
class scanner_listener;

struct JSONXX_DECLSPEC stack_parser_state {
	scanner_listener_stack& stack;
	object_listener& listener;

	stack_parser_state(scanner_listener_stack& stack_, object_listener& listener_) :
		stack(stack_),
		listener(listener_)
	{
	}
};

struct JSONXX_DECLSPEC parser2_state : stack_parser_state {
	scanner_listener* expect_value;

	parser2_state(
		scanner_listener_stack& stack_,
		object_listener& listener_,
		scanner_listener* expect_value_
	) :
		stack_parser_state(stack_, listener_),
		expect_value(expect_value_)
	{
	}
};

} // namespace jsonxx

#undef JSONXX_DECLSPEC

#endif // JSONXX_PARSER2_STATE_HPP
