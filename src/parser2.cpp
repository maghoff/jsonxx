#include <sstream>
#include <stdexcept>
#include "scanner_listener_stack.hpp"
#include "object_listener.hpp"
#include "parser2.hpp"

namespace jsonxx {

namespace {

namespace listener {

struct expect_value;

struct expect_start_object;
struct expect_key_or_end_object;
struct expect_key;
struct expect_colon;
struct expect_comma_or_end_object;

struct expect_start_array;
struct expect_value_or_end_array;
struct expect_comma_or_end_array;

struct parser2_state {
	scanner_listener_stack& stack;
	object_listener& listener;

	scanner_listener& expect_value;

	scanner_listener& expect_start_object;
	scanner_listener& expect_key_or_end_object;
	scanner_listener& expect_key;
	scanner_listener& expect_colon;
	scanner_listener& expect_comma_or_end_object;

	scanner_listener& expect_start_array;
	scanner_listener& expect_value_or_end_array;
	scanner_listener& expect_comma_or_end_array;

	parser2_state(
		scanner_listener_stack& stack_,
		object_listener& listener_,
		scanner_listener& expect_value_,
		scanner_listener& expect_start_object_,
		scanner_listener& expect_key_or_end_object_,
		scanner_listener& expect_key_,
		scanner_listener& expect_colon_,
		scanner_listener& expect_comma_or_end_object_,
		scanner_listener& expect_start_array_,
		scanner_listener& expect_value_or_end_array_,
		scanner_listener& expect_comma_or_end_array_
	) :
		stack(stack_),
		listener(listener_),
		expect_value(expect_value_),
		expect_start_object(expect_start_object_),
		expect_key_or_end_object(expect_key_or_end_object_),
		expect_key(expect_key_),
		expect_colon(expect_colon_),
		expect_comma_or_end_object(expect_comma_or_end_object_),
		expect_start_array(expect_start_array_),
		expect_value_or_end_array(expect_value_or_end_array_),
		expect_comma_or_end_array(expect_comma_or_end_array_)
	{
	}
};

struct error_fallback : scanner_listener {
	void start_object() { throw std::runtime_error("Lulz"); }
	void end_object() { throw std::runtime_error("Lulz"); }

	void start_array() { throw std::runtime_error("Lulz"); }
	void end_array() { throw std::runtime_error("Lulz"); }

	void comma() { throw std::runtime_error("Lulz"); }
	void colon() { throw std::runtime_error("Lulz"); }

	void number(const std::string&) { throw std::runtime_error("Lulz"); }
	void string(const std::string&) { throw std::runtime_error("Lulz"); }
	void bool_true() { throw std::runtime_error("Lulz"); }
	void bool_false() { throw std::runtime_error("Lulz"); }
	void null() { throw std::runtime_error("Lulz"); }
};

struct expect_start_object : error_fallback {
	parser2_state& s;

	expect_start_object(parser2_state& s_) : s(s_) { }

	void start_object() {
		s.listener.start_object();
		s.stack.pop();
		s.stack.push(&s.expect_key_or_end_object);
	}
};

struct expect_key_or_end_object : error_fallback {
	parser2_state& s;

	expect_key_or_end_object(parser2_state& s_) : s(s_) { }

	void end_object() {
		s.listener.end_object();
		s.stack.pop();
	}

	void string(const std::string& key) {
		s.listener.key(key);
		s.stack.pop();
		s.stack.push(&s.expect_comma_or_end_object);
		s.stack.push(&s.expect_value);
		s.stack.push(&s.expect_colon);
	}
};

struct expect_key : error_fallback {
	parser2_state& s;

	expect_key(parser2_state& s_) : s(s_) { }

	void string(const std::string& key) {
		s.listener.key(key);
		s.stack.pop();
		s.stack.push(&s.expect_comma_or_end_object);
		s.stack.push(&s.expect_value);
		s.stack.push(&s.expect_colon);
	}
};

struct expect_colon : error_fallback {
	parser2_state& s;

	expect_colon(parser2_state& s_) : s(s_) { }

	void colon() {
		s.stack.pop();
	}
};

struct expect_comma_or_end_object : error_fallback {
	parser2_state& s;

	expect_comma_or_end_object(parser2_state& s_) : s(s_) { }

	void end_object() {
		s.listener.end_object();
		s.stack.pop();
	}

	void comma() {
		s.stack.pop();
		s.stack.push(&s.expect_key);
	}
};

struct expect_value : error_fallback {
	parser2_state& s;

	expect_value(parser2_state& s_) : s(s_) { }

	void start_object() {
		s.stack.pop();
		s.stack.push(&s.expect_start_object);
		s.stack.start_object();
	}

	void start_array() {
		s.stack.pop();
		s.stack.push(&s.expect_start_array);
		s.stack.start_array();
	}

	void number(const std::string& value) {
		std::stringstream ss(value);
		double d;
		ss >> d;
		s.listener.value(d);
		s.stack.pop();
	}

	void string(const std::string& value) {
		s.listener.value(value);
		s.stack.pop();
	}

	void bool_true() {
		s.listener.value_bool(true);
		s.stack.pop();
	}

	void bool_false() {
		s.listener.value_bool(false);
		s.stack.pop();
	}

	void null() {
		s.listener.value_null();
		s.stack.pop();
	}
};

}

}

struct parser2::impl {
	scanner_listener_stack stack;
	object_listener& listener;

	listener::error_fallback error_state;

	listener::expect_value expect_value;

	listener::expect_start_object expect_start_object;
	listener::expect_key_or_end_object expect_key_or_end_object;
	listener::expect_key expect_key;
	listener::expect_colon expect_colon;
	listener::expect_comma_or_end_object expect_comma_or_end_object;

	listener::error_fallback expect_start_array;
	listener::error_fallback expect_value_or_end_array;
	listener::error_fallback expect_comma_or_end_array;

	listener::parser2_state state;

	impl(object_listener& listener_) :
		listener(listener_),
		expect_value(state),
		expect_start_object(state),
		expect_key_or_end_object(state),
		expect_key(state),
		expect_colon(state),
		expect_comma_or_end_object(state),
		state(
			stack,
			listener,
			expect_value,
			expect_start_object,
			expect_key_or_end_object,
			expect_key,
			expect_colon,
			expect_comma_or_end_object,
			expect_start_array,
			expect_value_or_end_array,
			expect_comma_or_end_array
		)
	{
	}
};


parser2::parser2(object_listener& listener_) {
	d = new impl(listener_);
	d->stack.push(&d->error_state);
	d->stack.push(&d->expect_value);
}

parser2::~parser2() {
	delete d;
}

scanner_listener& parser2::lol_get_scanner_listener() {
	return d->stack;
}

bool parser2::is_done() const {
	return d->stack.size() == 1;
}

}
