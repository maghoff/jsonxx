#include <sstream>
#include "parse_error.hpp"

namespace jsonxx {

static std::string make_message(const std::string& expected_description, const std::string& found_description) {
	std::stringstream ss;
	ss << "Expected " << expected_description << ", found " << found_description;
	return ss.str();
}

parse_error::parse_error(const std::string& expected_description_, const std::string& found_description_) :
	std::runtime_error(make_message(expected_description_, found_description_)),
	expected_description(expected_description_),
	found_description(found_description_)
{
}

parse_error::~parse_error() throw() {
}

}
