#include <sstream>
#include <stdexcept>
#include "scanner_listener_stack.hpp"
#include "object_listener.hpp"
#include "util.hpp"
#include "parser2.hpp"
#include "parser2_state.hpp"

#include "parse_error.hpp"
#include "error_fallback.hpp"
#include "expect_start_object.hpp"
#include "expect_key_or_end_object.hpp"
#include "expect_key.hpp"
#include "expect_colon.hpp"
#include "expect_comma_or_end_object.hpp"
#include "expect_start_array.hpp"
#include "expect_value_or_end_array.hpp"
#include "expect_comma_or_end_array.hpp"
#include "expect_value.hpp"
#include "expect_value_stream.hpp"

namespace jsonxx {

struct parser2::impl {
	scanner_listener_stack stack;
	object_listener& listener;

	error_fallback error_state;

	expect_value expect_value_state;

	expect_value_stream expect_value_stream_state;

	expect_start_object expect_start_object_state;
	expect_key_or_end_object expect_key_or_end_object_state;
	expect_key expect_key_state;
	expect_colon expect_colon_state;
	expect_comma_or_end_object expect_comma_or_end_object_state;

	expect_start_array expect_start_array_state;
	expect_value_or_end_array expect_value_or_end_array_state;
	expect_comma_or_end_array expect_comma_or_end_array_state;

	parser2_state state;

	impl(object_listener& listener_) :
		listener(listener_),
		error_state("EOF"),
		expect_value_state(state),
		expect_value_stream_state(state),
		expect_start_object_state(state),
		expect_key_or_end_object_state(state),
		expect_key_state(state),
		expect_colon_state(state),
		expect_comma_or_end_object_state(state),
		expect_start_array_state(state),
		expect_value_or_end_array_state(state),
		expect_comma_or_end_array_state(state),
		state(
			stack,
			listener,
			&expect_value_state,
			&expect_value_stream_state,
			&expect_start_object_state,
			&expect_key_or_end_object_state,
			&expect_key_state,
			&expect_colon_state,
			&expect_comma_or_end_object_state,
			&expect_start_array_state,
			&expect_value_or_end_array_state,
			&expect_comma_or_end_array_state
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
