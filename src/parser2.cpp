#include <sstream>
#include <stdexcept>
#include "scanner_listener_stack.hpp"
#include "object_listener.hpp"
#include "util.hpp"
#include "parser2.hpp"
#include "parser2_state.hpp"

namespace jsonxx {

namespace {

namespace listener {

class parse_error : public std::runtime_error {
	std::string expected_description, found_description;

public:
	parse_error(const std::string& expected_description, const std::string& found_description);
	~parse_error() throw();
};

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

class error_fallback : public scanner_listener {
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

error_fallback::error_fallback(const std::string& expected_description_) :
	expected_description(expected_description_)
{
}

void error_fallback::throw_simple_error(const std::string& found) {
	throw parse_error(expected_description, found);
}

void error_fallback::start_object() { throw_simple_error("{"); }
void error_fallback::end_object() { throw_simple_error("}"); }

void error_fallback::start_array() { throw_simple_error("["); }
void error_fallback::end_array() { throw_simple_error("]"); }

void error_fallback::comma() { throw_simple_error(","); }
void error_fallback::colon() { throw_simple_error(":"); }

void error_fallback::number(const std::string& number) {
	std::stringstream ss;
	ss << "number (" << number << ')';
	throw parse_error(expected_description, ss.str());
}

void error_fallback::string(const std::string& string) {
	std::stringstream ss;
	ss << "string (";
	write_quoted_string(ss, string);
	ss << ')';
	throw parse_error(expected_description, ss.str());
}

void error_fallback::bool_true() { throw_simple_error("true"); }
void error_fallback::bool_false() { throw_simple_error("false"); }
void error_fallback::null() { throw_simple_error("null"); }

struct expect_start_object : error_fallback {
	parser2_state& s;

	expect_start_object(parser2_state& s_) : error_fallback("{"), s(s_) { }

	void start_object() {
		s.listener.start_object();
		s.stack.pop();
		s.stack.push(s.expect_key_or_end_object);
	}
};

struct expect_key_or_end_object : error_fallback {
	parser2_state& s;

	expect_key_or_end_object(parser2_state& s_) : error_fallback("key or }"), s(s_) { }

	void end_object() {
		s.listener.end_object();
		s.stack.pop();
	}

	void string(const std::string& key) {
		s.listener.key(key);
		s.stack.pop();
		s.stack.push(s.expect_comma_or_end_object);
		s.stack.push(s.expect_value);
		s.stack.push(s.expect_colon);
	}
};

struct expect_key : error_fallback {
	parser2_state& s;

	expect_key(parser2_state& s_) : error_fallback("key"), s(s_) { }

	void string(const std::string& key) {
		s.listener.key(key);
		s.stack.pop();
		s.stack.push(s.expect_comma_or_end_object);
		s.stack.push(s.expect_value);
		s.stack.push(s.expect_colon);
	}
};

struct expect_colon : error_fallback {
	parser2_state& s;

	expect_colon(parser2_state& s_) : error_fallback(":"), s(s_) { }

	void colon() {
		s.stack.pop();
	}
};

struct expect_comma_or_end_object : error_fallback {
	parser2_state& s;

	expect_comma_or_end_object(parser2_state& s_) : error_fallback(", or }"), s(s_) { }

	void end_object() {
		s.listener.end_object();
		s.stack.pop();
	}

	void comma() {
		s.stack.pop();
		s.stack.push(s.expect_key);
	}
};

struct expect_start_array : error_fallback {
	parser2_state& s;

	expect_start_array(parser2_state& s_) : error_fallback("["), s(s_) { }

	void start_array() {
		s.listener.start_array();
		s.stack.pop();
		s.stack.push(s.expect_value_or_end_array);
	}
};

struct expect_value_or_end_array : error_fallback {
	parser2_state& s;

	expect_value_or_end_array(parser2_state& s_) : error_fallback("value or ]"), s(s_) { }

	void end_array() {
		s.listener.end_array();
		s.stack.pop();
	}

	void start_object() { fall_back_to_value(); s.stack.start_object(); }
	void start_array() { fall_back_to_value(); s.stack.start_array(); }
	void number(const std::string& v) { fall_back_to_value(); s.stack.number(v); }
	void string(const std::string& v) { fall_back_to_value(); s.stack.string(v); }
	void bool_true() { fall_back_to_value(); s.stack.bool_true(); }
	void bool_false() { fall_back_to_value(); s.stack.bool_false(); }
	void null() { fall_back_to_value(); s.stack.null(); }

private:
	void fall_back_to_value() {
		s.stack.pop();
		s.stack.push(s.expect_comma_or_end_array);
		s.stack.push(s.expect_value);
	}
};

struct expect_comma_or_end_array : error_fallback {
	parser2_state& s;

	expect_comma_or_end_array(parser2_state& s_) : error_fallback(", or ]"), s(s_) { }

	void end_array() {
		s.listener.end_array();
		s.stack.pop();
	}

	void comma() {
		s.stack.pop();
		s.stack.push(s.expect_comma_or_end_array);
		s.stack.push(s.expect_value);
	}
};

struct expect_value : error_fallback {
	parser2_state& s;

	expect_value(parser2_state& s_) : error_fallback("value"), s(s_) { }

	void start_object() {
		s.stack.pop();
		s.stack.push(s.expect_start_object);
		s.stack.start_object();
	}

	void start_array() {
		s.stack.pop();
		s.stack.push(s.expect_start_array);
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

struct expect_value_stream : error_fallback {
	parser2_state& s;

	expect_value_stream(parser2_state& s_) : error_fallback("value"), s(s_) { }

	void start_object() {
		s.stack.push(s.expect_value);
		s.stack.start_object();
	}

	void start_array() {
		s.stack.push(s.expect_value);
		s.stack.start_array();
	}

	void number(const std::string& value) {
		s.stack.push(s.expect_value);
		s.stack.number(value);
	}

	void string(const std::string& value) {
		s.stack.push(s.expect_value);
		s.stack.string(value);
	}

	void bool_true() {
		s.stack.push(s.expect_value);
		s.stack.bool_true();
	}

	void bool_false() {
		s.stack.push(s.expect_value);
		s.stack.bool_false();
	}

	void null() {
		s.stack.push(s.expect_value);
		s.stack.null();
	}
};

}

}

struct parser2::impl {
	scanner_listener_stack stack;
	object_listener& listener;

	listener::error_fallback error_state;

	listener::expect_value expect_value;

	listener::expect_value_stream expect_value_stream;

	listener::expect_start_object expect_start_object;
	listener::expect_key_or_end_object expect_key_or_end_object;
	listener::expect_key expect_key;
	listener::expect_colon expect_colon;
	listener::expect_comma_or_end_object expect_comma_or_end_object;

	listener::expect_start_array expect_start_array;
	listener::expect_value_or_end_array expect_value_or_end_array;
	listener::expect_comma_or_end_array expect_comma_or_end_array;

	parser2_state state;

	impl(object_listener& listener_) :
		listener(listener_),
		error_state("EOF"),
		expect_value(state),
		expect_value_stream(state),
		expect_start_object(state),
		expect_key_or_end_object(state),
		expect_key(state),
		expect_colon(state),
		expect_comma_or_end_object(state),
		expect_start_array(state),
		expect_value_or_end_array(state),
		expect_comma_or_end_array(state),
		state(
			stack,
			listener,
			&expect_value,
			&expect_value_stream,
			&expect_start_object,
			&expect_key_or_end_object,
			&expect_key,
			&expect_colon,
			&expect_comma_or_end_object,
			&expect_start_array,
			&expect_value_or_end_array,
			&expect_comma_or_end_array
		)
	{
	}
};


parser2::parser2(object_listener& listener_, scanner_listener* parser2_state::*initial_state) {
	d = new impl(listener_);
	try {
		d->stack.push(&d->error_state);
		d->stack.push(d->state.*initial_state);
	}
	catch (...) {
		delete d;
		throw;
	}
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
