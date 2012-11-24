#ifndef JSONXX_EXPECT_VALUE_HPP
#define JSONXX_EXPECT_VALUE_HPP

#include "error_fallback.hpp"

#include "declspec.hpp"

namespace jsonxx {

class parser2_state;

struct JSONXX_DECLSPEC expect_value : error_fallback {
	parser2_state& s;

	expect_value(parser2_state&);

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
