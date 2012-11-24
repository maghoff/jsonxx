#ifndef JSONXX_EXPECT_KEY_OR_END_OBJECT_HPP
#define JSONXX_EXPECT_KEY_OR_END_OBJECT_HPP

#include "error_fallback.hpp"

#include "declspec.hpp"

namespace jsonxx {

class parser2_state;

struct JSONXX_DECLSPEC expect_key_or_end_object : error_fallback {
	parser2_state& s;

	expect_key_or_end_object(parser2_state& s_);

	void end_object();

	void string(const std::string& key);
};

} // namespace jsonxx

#undef JSONXX_DECLSPEC

#endif // JSONXX_EXPECT_KEY_OR_END_OBJECT_HPP
