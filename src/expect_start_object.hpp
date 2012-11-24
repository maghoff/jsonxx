#ifndef JSONXX_EXPECT_START_OBJECT_HPP
#define JSONXX_EXPECT_START_OBJECT_HPP

#include "error_fallback.hpp"

#include "declspec.hpp"

namespace jsonxx {

class parser2_state;

struct JSONXX_DECLSPEC expect_start_object : error_fallback {
	parser2_state& s;

	expect_start_object(parser2_state&);

	void start_object();
};

} // namespace jsonxx

#undef JSONXX_DECLSPEC

#endif // JSONXX_EXPECT_START_OBJECT_HPP
