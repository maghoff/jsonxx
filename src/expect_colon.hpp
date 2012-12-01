#ifndef JSONXX_EXPECT_COLON_HPP
#define JSONXX_EXPECT_COLON_HPP

#include "error_scanner_listener.hpp"

#include "declspec.hpp"

namespace jsonxx {

class JSONXX_DECLSPEC expect_colon : public error_scanner_listener {
	parser2_state& s;

public:
	expect_colon(parser2_state&);

	void colon();
};

} // namespace jsonxx

#undef JSONXX_DECLSPEC

#endif // JSONXX_EXPECT_COLON_HPP
