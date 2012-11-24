#ifndef JSONXX_PARSER2_STATE_HPP
#define JSONXX_PARSER2_STATE_HPP

#include "declspec.hpp"

namespace jsonxx {

class scanner_listener_stack;
class object_listener;
class scanner_listener;

struct JSONXX_DECLSPEC parser2_state {
	scanner_listener_stack& stack;
	object_listener& listener;

	scanner_listener* expect_value;
	scanner_listener* expect_value_stream;

	scanner_listener* expect_start_object;
	scanner_listener* expect_key_or_end_object;
	scanner_listener* expect_key;
	scanner_listener* expect_colon;
	scanner_listener* expect_comma_or_end_object;

	scanner_listener* expect_start_array;

	parser2_state(
		scanner_listener_stack& stack_,
		object_listener& listener_,
		scanner_listener* expect_value_,
		scanner_listener* expect_value_stream_,
		scanner_listener* expect_start_object_,
		scanner_listener* expect_key_or_end_object_,
		scanner_listener* expect_key_,
		scanner_listener* expect_colon_,
		scanner_listener* expect_comma_or_end_object_,
		scanner_listener* expect_start_array_
	) :
		stack(stack_),
		listener(listener_),
		expect_value(expect_value_),
		expect_value_stream(expect_value_stream_),
		expect_start_object(expect_start_object_),
		expect_key_or_end_object(expect_key_or_end_object_),
		expect_key(expect_key_),
		expect_colon(expect_colon_),
		expect_comma_or_end_object(expect_comma_or_end_object_),
		expect_start_array(expect_start_array_)
	{
	}
};

} // namespace jsonxx

#undef JSONXX_DECLSPEC

#endif // JSONXX_PARSER2_STATE_HPP
