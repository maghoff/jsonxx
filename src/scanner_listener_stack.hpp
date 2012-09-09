#ifndef JSONXX_SCANNER_LISTENER_STACK_HPP
#define JSONXX_SCANNER_LISTENER_STACK_HPP

#include <stack>
#include "scanner_listener.hpp"

#include "declspec.hpp"

namespace jsonxx {

class JSONXX_DECLSPEC scanner_listener_stack : public scanner_listener {
	typedef std::stack<scanner_listener*> underlying_stack;
	std::stack<scanner_listener*> stack;

public:
	scanner_listener_stack();
	~scanner_listener_stack();

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

	void push(scanner_listener*);
	void pop();
	underlying_stack::size_type size();
};

} // namespace jsonxx

#undef JSONXX_DECLSPEC

#endif // JSONXX_SCANNER_LISTENER_STACK_HPP
