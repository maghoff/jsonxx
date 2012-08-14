#include <iostream>
#include <sstream>
#include "test_util.hpp"
#include "scanner.hpp"
#include "scanner_listener.hpp"

namespace {

struct test_scanner_listener : jsonxx::scanner_listener {
	std::stringstream event_stream;

	void start_object() { event_stream << '{'; }
	void end_object() { event_stream << '}'; }

	void start_array() { event_stream << '['; }
	void end_array() { event_stream << ']'; }

	void comma() { event_stream << ','; }
	void colon() { event_stream << ':'; }

	void number() { event_stream << 'n'; }
	void string() { event_stream << 's'; }
	void boolean() { event_stream << 'b'; }
	void null() { event_stream << '0'; }
};


bool scanner_simple_object() {
	bool ok = true;

	test_scanner_listener listener;
	jsonxx::scanner s(listener);

	s.scan("{}");

	CHECK_EQUAL(listener.event_stream.str(), "{}");

	return ok;
}

bool scanner_ignores_whitespace() {
	bool ok = true;

	test_scanner_listener listener;
	jsonxx::scanner s(listener);

	s.scan(" { \n} \r\n,");

	CHECK_EQUAL(listener.event_stream.str(), "{},");

	return ok;
}

}

bool scanner_tests() {
    bool ok = true;

    ok &= EXEC(scanner_simple_object);
    ok &= EXEC(scanner_ignores_whitespace);

    return ok;
}
