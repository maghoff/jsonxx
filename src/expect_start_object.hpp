#ifndef JSONXX_EXPECT_START_OBJECT_HPP
#define JSONXX_EXPECT_START_OBJECT_HPP

#include "error_scanner_listener.hpp"
#include "expect_key_or_end_object.hpp"

#include "declspec.hpp"

namespace jsonxx {

class parser2_state;

class JSONXX_DECLSPEC expect_start_object : public error_scanner_listener {
	parser2_state& s;
	expect_key_or_end_object expect_key_or_end_object_state;

public:
	expect_start_object(parser2_state&);

	void start_object();
};

} // namespace jsonxx

#undef JSONXX_DECLSPEC

#endif // JSONXX_EXPECT_START_OBJECT_HPP
