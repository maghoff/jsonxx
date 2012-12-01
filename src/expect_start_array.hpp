#ifndef JSONXX_EXPECT_START_ARRAY_HPP
#define JSONXX_EXPECT_START_ARRAY_HPP

#include "error_scanner_listener.hpp"
#include "expect_value_or_end_array.hpp"

#include "declspec.hpp"

namespace jsonxx {

class parser2_state;

class JSONXX_DECLSPEC expect_start_array : public error_scanner_listener {
	parser2_state& s;
	expect_value_or_end_array expect_value_or_end_array_state;

public:
	expect_start_array(parser2_state&);

	void start_array();
};

} // namespace jsonxx

#undef JSONXX_DECLSPEC

#endif // JSONXX_EXPECT_START_ARRAY_HPP
