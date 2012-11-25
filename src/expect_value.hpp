#ifndef JSONXX_EXPECT_VALUE_HPP
#define JSONXX_EXPECT_VALUE_HPP

#include "error_fallback.hpp"
#include "expect_start_object.hpp"
#include "expect_start_array.hpp"
#include "parser2_state.hpp"

#include "declspec.hpp"

namespace jsonxx {

class JSONXX_DECLSPEC expect_value : public error_fallback {
	parser2_state s;
	expect_start_object expect_start_object_state;
	expect_start_array expect_start_array_state;

public:
	expect_value(stack_parser_state&);

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

#endif // JSONXX_EXPECT_VALUE_HPP
