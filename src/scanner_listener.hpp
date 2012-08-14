#ifndef JSONXX_SCANNER_LISTENER_HPP
#define JSONXX_SCANNER_LISTENER_HPP

#include "declspec.hpp"

namespace jsonxx {

class JSONXX_DECLSPEC scanner_listener {
public:
	scanner_listener();
	virtual ~scanner_listener();

	virtual void start_object() = 0;
	virtual void end_object() = 0;

	virtual void start_array() = 0;
	virtual void end_array() = 0;

	virtual void comma() = 0;
	virtual void colon() = 0;

	virtual void number() = 0;
	virtual void string() = 0;
	virtual void boolean() = 0;
	virtual void null() = 0;

	// or just one for terminal_value?
};

} // namespace jsonxx

#undef JSONXX_DECLSPEC

#endif // JSONXX_SCANNER_LISTENER_HPP
