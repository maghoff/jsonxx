#ifndef PARSE_ERROR_HPP
#define PARSE_ERROR_HPP

#include <string>
#include <stdexcept>

#include "declspec.hpp"

namespace jsonxx {

class JSONXX_DECLSPEC parse_error : public std::runtime_error {
	std::string expected_description, found_description;

public:
	parse_error(const std::string& expected_description, const std::string& found_description);
	~parse_error() throw();
};

} // namespace jsonxx

#undef JSONXX_DECLSPEC

#endif // PARSE_ERROR_HPP
