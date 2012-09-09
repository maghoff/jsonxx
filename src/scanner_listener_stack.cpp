#include "scanner_listener_stack.hpp"

namespace jsonxx {

scanner_listener_stack::scanner_listener_stack() { }

scanner_listener_stack::~scanner_listener_stack() { }

void scanner_listener_stack::start_object() {
	stack.top()->start_object();
}

void scanner_listener_stack::end_object() {
	stack.top()->end_object();
}

void scanner_listener_stack::start_array() {
	stack.top()->start_array();
}

void scanner_listener_stack::end_array() {
	stack.top()->end_array();
}

void scanner_listener_stack::comma() {
	stack.top()->comma();
}

void scanner_listener_stack::colon() {
	stack.top()->colon();
}

void scanner_listener_stack::number(const std::string& number) {
	stack.top()->number(number);
}

void scanner_listener_stack::string(const std::string& string) {
	stack.top()->string(string);
}

void scanner_listener_stack::bool_true() {
	stack.top()->bool_true();
}

void scanner_listener_stack::bool_false() {
	stack.top()->bool_false();
}

void scanner_listener_stack::null() {
	stack.top()->null();
}

void scanner_listener_stack::push(scanner_listener* listener) {
	stack.push(listener);
}

void scanner_listener_stack::pop() {
	stack.pop();
}

scanner_listener_stack::underlying_stack::size_type scanner_listener_stack::size() {
	return stack.size();
}

}
