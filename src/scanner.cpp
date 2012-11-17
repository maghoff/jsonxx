#include <cassert>
#include <sstream>
#include <stdexcept>
#include "scanner_listener.hpp"
#include "scanner.hpp"
#include "util.hpp"

namespace jsonxx {

namespace {

char true_literal[] = "true";
char false_literal[] = "false";
char null_literal[] = "null";

}

scanner::scanner(scanner_listener& listener_) :
	listener(listener_),
	state(&scanner::root_level)
{
}

scanner::~scanner() { }

void scanner::scan_literal(const char* begin, const char* end, literal_completion callback) {
	scanning_literal_pos = begin;
	scanning_literal_end = end;
	scanning_literal_completion = callback;
	state = &scanner::literal;
}

const char* scanner::error_state(const char* begin, const char* end) {
	return end;
}

const char* scanner::root_level(const char* begin, const char* end) {
	char look = *begin;

	#define CHECK_LITERAL(literal_name, callback_name) \
		if (look == literal_name[0]) scan_literal(literal_name + 1, literal_name + sizeof(literal_name) - 1, &scanner_listener::callback_name);

	if (look == '{') listener.start_object();
	else if (look == '}') listener.end_object();
	else if (look == '[') listener.start_array();
	else if (look == ']') listener.end_array();
	else if (look == ',') listener.comma();
	else if (look == ':') listener.colon();
	else CHECK_LITERAL(true_literal, bool_true)
	else CHECK_LITERAL(false_literal, bool_false)
	else CHECK_LITERAL(null_literal, null)
	else if ((look == ' ') || (look == '\t') || (look == '\n') || (look == '\r')) { /* ignore whitespace */ }
	else if ((look == '-') || ((look >= '0') && (look <= '9'))) {
		token_buffer.str(std::string());
		state = &scanner::number_start;
		if (look == '-') {
			token_buffer << look;
			return begin + 1;
		} else {
			return begin;
		}
	} else if (look == '\"') {
		token_buffer.str(std::string());
		state = &scanner::in_string;
	} else {
		state = &scanner::error_state;
		throw syntax_error(std::string("Unexpected byte in input: ") + look);
	}

	return begin + 1;
}

const char* scanner::in_string(const char* begin, const char* end) {
	const char* next = std::find_if(begin, end, &is_string_special_char);
	token_buffer.write(begin, next - begin);

	const char* position = next;
	if (position != end) {
		char look = *next;
		++position;

		if (look == '"') {
			listener.string(token_buffer.str());
			state = &scanner::root_level;
		} else if (look == '\\') {
			state = &scanner::string_escape_sequence;
		} else {
			assert((0 <= look) && (look < 0x20));
			state = &scanner::error_state;
			throw syntax_error(std::string("Unexpected byte in string: ") + look);
		}
	}

	return position;
}

const char* scanner::string_escape_sequence(const char* begin, const char* end) {
	char look = *begin;

	state = &scanner::in_string;

	if (look == '"') token_buffer << '"';
	else if (look == '\\') token_buffer << '\\';
	else if (look == '/') token_buffer << '/';
	else if (look == 'b') token_buffer << '\b';
	else if (look == 'f') token_buffer << '\f';
	else if (look == 'n') token_buffer << '\n';
	else if (look == 'r') token_buffer << '\r';
	else if (look == 't') token_buffer << '\t';
	else if (look == 'u') {
		escape_nibbles_left = 4;
		current_escape_value = 0;
		state = &scanner::unicode_escape_sequence;
	} else {
		state = &scanner::error_state;
		throw syntax_error(std::string("Invalid escape sequence in string: \\") + look);
	}

	return begin + 1;
}

const char* scanner::unicode_escape_sequence(const char* begin, const char* end) {
	char look = *begin;

	int val;

	if (look >= 'a' && look <= 'f') {
		val = look - 'a' + 10;
	} else if (look >= 'A' && look <= 'F') {
		val = look - 'A' + 10;
	} else if (look >= '0' && look <= '9') {
		val = look - '0';
	} else {
		state = &scanner::error_state;
		throw syntax_error(std::string("Invalid character in unicode escape sequence: ") + look);
	}

	current_escape_value = (current_escape_value << 4) + val;
	escape_nibbles_left--;

	if (escape_nibbles_left == 0) {
		bool is_surrogate = (current_escape_value & 0xD800) == 0xD800;
		bool is_lead_surrogate = (current_escape_value & 0xFC00) == 0xD800;
		bool is_trail_surrogate = (current_escape_value & 0xFC00) == 0xDC00;

		assert(is_surrogate == (is_lead_surrogate || is_trail_surrogate));
		assert(is_surrogate == (is_lead_surrogate != is_trail_surrogate));

		if (!is_surrogate) {
			write_code_point_as_utf8(token_buffer, current_escape_value);
		} else if (is_lead_surrogate) {
			lead_surrogate = current_escape_value;
		} else if (is_trail_surrogate) {
			uint32_t low_bits = current_escape_value & 0x03FF;
			uint32_t high_bits = lead_surrogate & 0x03FF;
			uint32_t value = low_bits + (high_bits << 10) + 0x010000;
			write_code_point_as_utf8(token_buffer, value);
		} else {
			assert(false);
		}

		state = &scanner::in_string;
	}

	return begin + 1;
}

const char* scanner::literal(const char* begin, const char* end) {
	const char* pos = begin;
	while ((pos != end) && (scanning_literal_pos != scanning_literal_end)) {
		if (*pos != *scanning_literal_pos) {
			state = &scanner::error_state;
			throw syntax_error(std::string("Unexpected byte in literal: ") + (*pos) + " Expected: " + (*scanning_literal_pos));
		}
		++pos;
		++scanning_literal_pos;
	}

	if (scanning_literal_pos == scanning_literal_end) {
		(listener.*scanning_literal_completion)();
		state = &scanner::root_level;
	}

	return pos;
}

const char* scanner::number_start(const char* begin, const char* end) {
	char look = *begin;
	token_buffer << look;

	if (look == '0') state = &scanner::number_after_zero_integral_part;
	else if ((look >= '1') && (look <= '9')) state = &scanner::number_integral_part;
	else {
		state = &scanner::error_state;
		throw syntax_error(std::string("Unexpected byte in number: ") + look + " Expected digit");
	}

	return begin + 1;
}

const char* scanner::number_integral_part(const char* begin, const char* end) {
	const char* pos = begin;

	while ((pos != end) && ((*pos) >= '0') && ((*pos) <= '9')) ++pos;

	token_buffer.write(begin, pos - begin);

	if (pos != end) state = &scanner::number_after_integral_part;

	return pos;
}

const char* scanner::number_after_zero_integral_part(const char* begin, const char* end) {
	char look = *begin;

	if ((look >= '0') && (look <= '9')) {
		state = &scanner::error_state;
		throw syntax_error("Numbers cannot be prefixed by zero");
	} else {
		state = &scanner::number_after_integral_part;
	}

	return begin;
}

const char* scanner::number_after_integral_part(const char* begin, const char* end) {
	char look = *begin;

	if (look == '.') {
		token_buffer << look;
		state = &scanner::number_decimal_part;
		return begin + 1;
	} else {
		state = &scanner::number_after_decimal_part;
		return begin;
	}
}

const char* scanner::number_decimal_part(const char* begin, const char* end) {
	char look = *begin;

	if ((look >= '0') && (look <= '9')) {
		state = &scanner::number_in_decimal_part;
	} else {
		state = &scanner::error_state;
		throw syntax_error(std::string("Expected digits after decimal point. Got ") + look);
	}

	return begin;
}

const char* scanner::number_in_decimal_part(const char* begin, const char* end) {
	const char* pos = begin;

	while ((pos != end) && ((*pos) >= '0') && ((*pos) <= '9')) ++pos;

	token_buffer.write(begin, pos - begin);

	if (pos != end) state = &scanner::number_after_decimal_part;

	return pos;
}

const char* scanner::number_after_decimal_part(const char* begin, const char* end) {
	char look = *begin;

	if ((look == 'e') || (look == 'E')) {
		token_buffer << look;
		state = &scanner::number_exponent_maybe_prefix;
		return begin + 1;
	} else {
		listener.number(token_buffer.str());
		state = &scanner::root_level;
		return begin;
	}
}

const char* scanner::number_exponent_maybe_prefix(const char* begin, const char* end) {
	char look = *begin;

	state = &scanner::number_exponent_number;

	if ((look == '-') || (look == '+')) {
		token_buffer << look;
		return begin + 1;
	}

	return begin;
}

const char* scanner::number_exponent_number(const char* begin, const char* end) {
	char look = *begin;

	if ((look >= '0') && (look <= '9')) {
		state = &scanner::number_in_exponent_number;
	} else {
		state = &scanner::error_state;
		throw syntax_error(std::string("Expected digits after exponent marker. Got ") + look);
	}

	return begin;
}

const char* scanner::number_in_exponent_number(const char* begin, const char* end) {
	const char* pos = begin;

	while ((pos != end) && ((*pos) >= '0') && ((*pos) <= '9')) ++pos;

	token_buffer.write(begin, pos - begin);

	if (pos != end) {
		listener.number(token_buffer.str());
		state = &scanner::root_level;
	}

	return pos;
}

void scanner::scan(const char* begin, const char* end) {
	const char* position = begin;
	while (position != end) {
		position = (this->*state)(position, end);
	}
}

void scanner::scan(const std::string& str) {
	scan(str.data(), str.data() + str.length());
}

}
