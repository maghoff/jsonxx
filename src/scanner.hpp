#ifndef JSONXX_SCANNER_HPP
#define JSONXX_SCANNER_HPP

#include <sstream>
#include <stdexcept>
#include <string>
#include <stdint.h>

#include "declspec.hpp"

namespace jsonxx {

class scanner_listener;

class JSONXX_DECLSPEC scanner {
	scanner_listener& listener;

	typedef const char* (scanner::*state_function)(const char* begin, const char* end);
	state_function state;

	std::stringstream token_buffer;

	int escape_nibbles_left;
	uint16_t current_escape_value;
	uint16_t lead_surrogate;

	typedef void (scanner_listener::*literal_completion)();
	const char* scanning_literal_pos;
	const char* scanning_literal_end;
	literal_completion scanning_literal_completion;

	void scan_literal(const char* begin, const char* end, literal_completion callback);

	const char* error_state(const char* begin, const char* end);
	const char* root_level(const char* begin, const char* end);
	const char* in_string(const char* begin, const char* end);
	const char* string_escape_sequence(const char* begin, const char* end);
	const char* unicode_escape_sequence(const char* begin, const char* end);
	const char* expect_trail_surrogate_escape_backslash(const char* begin, const char* end);
	const char* expect_trail_surrogate_escape_u(const char* begin, const char* end);
	const char* literal(const char* begin, const char* end);

	const char* number_start(const char* begin, const char* end);
	const char* number_integral_part(const char* begin, const char* end);
	const char* number_after_zero_integral_part(const char* begin, const char* end);
	const char* number_after_integral_part(const char* begin, const char* end);
	const char* number_decimal_part(const char* begin, const char* end);
	const char* number_in_decimal_part(const char* begin, const char* end);
	const char* number_after_decimal_part(const char* begin, const char* end);
	const char* number_exponent_maybe_prefix(const char* begin, const char* end);
	const char* number_exponent_number(const char* begin, const char* end);
	const char* number_in_exponent_number(const char* begin, const char* end);

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
