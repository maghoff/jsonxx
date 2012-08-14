#include <cassert>
#include <sstream>
#include <stdexcept>
#include "scanner_listener.hpp"
#include "scanner.hpp"

namespace jsonxx {

scanner::scanner(scanner_listener& listener_) :
	listener(listener_)
{
}

scanner::~scanner() { }

void scanner::scan(const char* begin, const char* end) {
	if (begin == end) return;

	char look = *begin;

	if (look == '{') listener.start_object();
	else if (look == '}') listener.end_object();
	else if (look == '[') listener.start_array();
	else if (look == ']') listener.end_array();
	else if (look == ',') listener.comma();
	else if (look == ':') listener.colon();
	else if ((look == ' ') || (look == '\t') || (look == '\n') || (look == '\r')) { /* ignore whitespace */ }
	else {
		throw syntax_error(std::string("Unexpected byte in input: ") + look);
	}

	scan(begin + 1, end);
}

void scanner::scan(const std::string& str) {
	scan(str.data(), str.data() + str.length());
}

}
