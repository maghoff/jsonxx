#include <sstream>
#include "parse_error.hpp"
#include "util.hpp"
#include "error_scanner_listener.hpp"

namespace jsonxx {

error_scanner_listener::error_scanner_listener(const std::string& expected_description_) :
	expected_description(expected_description_)
{
}

void error_scanner_listener::throw_simple_error(const std::string& found) {
	throw parse_error(expected_description, found);
}

void error_scanner_listener::start_object() { throw_simple_error("{"); }
void error_scanner_listener::end_object() { throw_simple_error("}"); }

void error_scanner_listener::start_array() { throw_simple_error("["); }
void error_scanner_listener::end_array() { throw_simple_error("]"); }

void error_scanner_listener::comma() { throw_simple_error(","); }
void error_scanner_listener::colon() { throw_simple_error(":"); }

void error_scanner_listener::number(const std::string& number) {
	std::stringstream ss;
	ss << "number (" << number << ')';
	throw parse_error(expected_description, ss.str());
}

void error_scanner_listener::string(const std::string& string) {
	std::stringstream ss;
	ss << "string (";
	write_quoted_string(ss, string);
	ss << ')';
	throw parse_error(expected_description, ss.str());
}

void error_scanner_listener::bool_true() { throw_simple_error("true"); }
void error_scanner_listener::bool_false() { throw_simple_error("false"); }
void error_scanner_listener::null() { throw_simple_error("null"); }

} // namespace jsonxx
