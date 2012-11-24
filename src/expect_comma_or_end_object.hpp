#ifndef JSONXX_EXPECT_COMMA_OR_END_OBJECT_HPP
#define JSONXX_EXPECT_COMMA_OR_END_OBJECT_HPP

#include "error_fallback.hpp"

#include "declspec.hpp"

namespace jsonxx {

class parser2_state;

struct expect_comma_or_end_object : error_fallback {
	parser2_state& s;

	expect_comma_or_end_object(parser2_state&);

	void end_object();
	void comma();
};

} // namespace jsonxx

#undef JSONXX_DECLSPEC

#endif // JSONXX_EXPECT_COMMA_OR_END_OBJECT_HPP
