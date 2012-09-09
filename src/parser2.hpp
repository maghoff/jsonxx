#ifndef JSONXX_PARSER2_HPP
#define JSONXX_PARSER2_HPP

#include "declspec.hpp"

namespace jsonxx {

class object_listener;
class scanner_listener;

class JSONXX_DECLSPEC parser2 {
public:
	parser2(object_listener&);
	~parser2();

	struct impl;

	scanner_listener& lol_get_scanner_listener();

	bool is_done() const;

private:
	impl *d;
};

} // namespace jsonxx

#undef JSONXX_DECLSPEC

#endif // JSONXX_PARSER2_HPP
