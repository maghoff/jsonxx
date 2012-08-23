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
	else if (look == '\"') {
		parsing_string.str(std::string());
		state = &scanner::in_string;
	} else {
		state = &scanner::error_state;
		throw syntax_error(std::string("Unexpected byte in input: ") + look);
	}

	return begin + 1;
}

const char* scanner::in_string(const char* begin, const char* end) {
	const char* next = std::find_if(begin, end, &is_string_special_char);
	parsing_string.write(begin, next - begin);

	const char* position = next;
	if (position != end) {
		char look = *next;
		++position;

		if (look == '"') {
			listener.string(parsing_string.str());
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

	if (look == '"') parsing_string << '"';
	else if (look == '\\') parsing_string << '\\';
	else if (look == '/') parsing_string << '/';
	else if (look == 'b') parsing_string << '\b';
	else if (look == 'f') parsing_string << '\f';
	else if (look == 'n') parsing_string << '\n';
	else if (look == 'r') parsing_string << '\r';
	else if (look == 't') parsing_string << '\t';
	else if (look == 'u') {
		throw std::logic_error("Scanning of unicode escape sequences not implemented");
		//state = &scanner::unicode_escape_sequence;
	} else {
		state = &scanner::error_state;
		throw syntax_error(std::string("Invalid escape sequence in string: \\") + look);
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
