#ifndef JSONXX_EXPECT_KEY_OR_END_OBJECT_HPP
#define JSONXX_EXPECT_KEY_OR_END_OBJECT_HPP

#include "error_scanner_listener.hpp"
#include "expect_comma_or_end_object.hpp"
#include "expect_colon.hpp"

#include "declspec.hpp"

namespace jsonxx {

class parser2_state;

class JSONXX_DECLSPEC expect_key_or_end_object : public error_scanner_listener {
	parser2_state& s;
	expect_comma_or_end_object expect_comma_or_end_object_state;
	expect_colon expect_colon_state;

public:
	expect_key_or_end_object(parser2_state& s_);

	void end_object();

	void string(const std::string& key);
};

} // namespace jsonxx

#undef JSONXX_DECLSPEC

#endif // JSONXX_EXPECT_KEY_OR_END_OBJECT_HPP
