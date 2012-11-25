#ifndef JSONXX_EXPECT_VALUE_STREAM_HPP
#define JSONXX_EXPECT_VALUE_STREAM_HPP

#include "error_fallback.hpp"
#include "expect_value.hpp"

#include "declspec.hpp"

namespace jsonxx {

class stack_parser_state;

class JSONXX_DECLSPEC expect_value_stream : public error_fallback {
	stack_parser_state& s;
	expect_value expect_value_state;

public:
	expect_value_stream(stack_parser_state&);

	void start_object();
	void start_array();
	void number(const std::string&);
	void string(const std::string&);
	void bool_true();
	void bool_false();
	void null();
};

} // namespace jsonxx

#undef JSONXX_DECLSPEC

#endif // JSONXX_EXPECT_VALUE_STREAM_HPP
