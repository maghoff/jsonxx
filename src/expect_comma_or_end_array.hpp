#ifndef JSONXX_EXPECT_COMMA_OR_END_ARRAY_HPP
#define JSONXX_EXPECT_COMMA_OR_END_ARRAY_HPP

#include "error_fallback.hpp"

#include "declspec.hpp"

namespace jsonxx {

class parser2_state;

struct JSONXX_DECLSPEC expect_comma_or_end_array : error_fallback {
	parser2_state& s;

	expect_comma_or_end_array(parser2_state&);

	void end_array();

	void comma();
};

} // namespace jsonxx

#undef JSONXX_DECLSPEC

#endif // JSONXX_EXPECT_COMMA_OR_END_ARRAY_HPP
