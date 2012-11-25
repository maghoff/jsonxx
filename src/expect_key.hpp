#ifndef JSONXX_EXPECT_KEY_HPP
#define JSONXX_EXPECT_KEY_HPP

#include "error_fallback.hpp"

#include "declspec.hpp"

namespace jsonxx {

class JSONXX_DECLSPEC expect_key : public error_fallback {
	parser2_state& s;

public:
	expect_key(parser2_state& s_);

	void string(const std::string& key);
};

} // namespace jsonxx

#undef JSONXX_DECLSPEC

#endif // JSONXX_EXPECT_KEY_HPP
