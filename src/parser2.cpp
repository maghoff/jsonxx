#include <sstream>
#include <stdexcept>
#include "scanner_listener_stack.hpp"
#include "object_listener.hpp"
#include "util.hpp"
#include "parser2.hpp"
#include "parser2_state.hpp"
#include "scanner.hpp"

#include "parse_error.hpp"
#include "error_scanner_listener.hpp"
#include "expect_value.hpp"

namespace jsonxx {

struct parser2::impl {
	object_listener& listener;
	scanner_listener_stack stack;
	stack_parser_state state;

	scanner scanner_instance;

	error_scanner_listener error_state;
	expect_value expect_value_state;

	impl(object_listener& listener_) :
		listener(listener_),
		state(
			stack,
			listener
		),
		scanner_instance(stack),
		error_state("EOF"),
		expect_value_state(state)
	{
	}
};


parser2::parser2(object_listener& listener_) {
	d = new impl(listener_);
	try {
		d->stack.push(&d->error_state);
		d->stack.push(&d->expect_value_state);
	}
	catch (...) {
		delete d;
		throw;
	}
}

parser2::~parser2() {
	delete d;
}

bool parser2::is_done() const {
	return d->stack.size() == 1;
}

void parser2::parse(const char* begin, const char* end) {
	d->scanner_instance.scan(begin, end);
}

void parser2::parse(const std::string& data) {
	d->scanner_instance.scan(data);
}

}
