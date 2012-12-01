#ifndef JSONXX_EXPECT_VALUE_OR_END_ARRAY_HPP
#define JSONXX_EXPECT_VALUE_OR_END_ARRAY_HPP

#include "error_scanner_listener.hpp"
#include "expect_comma_or_end_array.hpp"

#include "declspec.hpp"

namespace jsonxx {

class parser2_state;

class JSONXX_DECLSPEC expect_value_or_end_array : public error_scanner_listener {
	parser2_state& s;
	expect_comma_or_end_array expect_comma_or_end_array_state;

public:
	expect_value_or_end_array(parser2_state&);

	void end_array();

	void start_object();
	void start_array();
	void number(const std::string&);
	void string(const std::string&);
	void bool_true();
	void bool_false();
	void null();

private:
	void fall_back_to_value();
};

} // namespace jsonxx

#undef JSONXX_DECLSPEC

#endif // JSONXX_EXPECT_VALUE_OR_END_ARRAY_HPP
