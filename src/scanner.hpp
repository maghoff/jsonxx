#ifndef JSONXX_SCANNER_HPP
#define JSONXX_SCANNER_HPP

#include <sstream>
#include <stdexcept>
#include <string>

#include "declspec.hpp"

namespace jsonxx {

class scanner_listener;

class JSONXX_DECLSPEC scanner {
	scanner_listener& listener;

	typedef const char* (scanner::*state_function)(const char* begin, const char* end);
	state_function state;

	std::stringstream parsing_string;

	const char* root_level(const char* begin, const char* end);
	const char* in_string(const char* begin, const char* end);

public:
	scanner(scanner_listener&);
	virtual ~scanner();

	void scan(const char* begin, const char* end);
	void scan(const std::string&);

	class syntax_error : public std::runtime_error {
	public:
		explicit syntax_error(const std::string& message) : std::runtime_error(message) { }
	};
};

} // namespace jsonxx

#undef JSONXX_DECLSPEC

#endif // JSONXX_SCANNER_HPP
