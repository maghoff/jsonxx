#include <sstream>
#include "parse_error.hpp"
#include "util.hpp"
#include "error_fallback.hpp"

namespace jsonxx {

error_fallback::error_fallback(const std::string& expected_description_) :
	expected_description(expected_description_)
{
}

void error_fallback::throw_simple_error(const std::string& found) {
	throw parse_error(expected_description, found);
}

void error_fallback::start_object() { throw_simple_error("{"); }
void error_fallback::end_object() { throw_simple_error("}"); }

void error_fallback::start_array() { throw_simple_error("["); }
void error_fallback::end_array() { throw_simple_error("]"); }

void error_fallback::comma() { throw_simple_error(","); }
void error_fallback::colon() { throw_simple_error(":"); }

void error_fallback::number(const std::string& number) {
	std::stringstream ss;
	ss << "number (" << number << ')';
	throw parse_error(expected_description, ss.str());
}

void error_fallback::string(const std::string& string) {
	std::stringstream ss;
	ss << "string (";
	write_quoted_string(ss, string);
	ss << ')';
	throw parse_error(expected_description, ss.str());
}

void error_fallback::bool_true() { throw_simple_error("true"); }
void error_fallback::bool_false() { throw_simple_error("false"); }
void error_fallback::null() { throw_simple_error("null"); }

} // namespace jsonxx
