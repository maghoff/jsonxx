#ifndef JSONXX_ERROR_FALLBACK_HPP
#define JSONXX_ERROR_FALLBACK_HPP

#include "scanner_listener.hpp"

#include "declspec.hpp"

namespace jsonxx {

class JSONXX_DECLSPEC error_fallback : public scanner_listener {
	std::string expected_description;
	void throw_simple_error(const std::string& found);

public:
	error_fallback(const std::string& expected_description);

	void start_object();
	void end_object();

	void start_array();
	void end_array();

	void comma();
	void colon();

	void number(const std::string&);
	void string(const std::string&);
	void bool_true();
	void bool_false();
	void null();
};

} // namespace jsonxx

#undef JSONXX_DECLSPEC

#endif // JSONXX_ERROR_FALLBACK_HPP
