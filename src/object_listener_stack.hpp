#ifndef JSONXX_OBJECT_LISTENER_STACK_HPP
#define JSONXX_OBJECT_LISTENER_STACK_HPP

#include <stack>
#include "object_listener.hpp"

#include "declspec.hpp"

namespace jsonxx {

class JSONXX_DECLSPEC object_listener_stack : public object_listener {
	std::stack<object_listener*> stack;

public:
	object_listener_stack();
	~object_listener_stack();

	void push(object_listener* parser);
	object_listener* top();
	void pop();

	void key(const std::string& key);

	void start_object();
	void end_object();

	void start_array();
	void end_array();

	void value(const std::string&);
	void value(int);
	void value(double);
	void value(bool_type);
	void value(null_type);
};

} // namespace jsonxx

#undef JSONXX_DECLSPEC

#endif // JSONXX_OBJECT_LISTENER_STACK_HPP
