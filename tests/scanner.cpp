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
	void string(const std::string& s) { event_stream << "s(" << s << ")"; }
	void boolean() { event_stream << 'b'; }
	void null() { event_stream << '0'; }
};


bool test_scanner_with_input_one_chunk(const std::string& test_name, int lineno, const std::string& input, const std::string& expected_output) {
	test_scanner_listener listener;
	jsonxx::scanner s(listener);

	s.scan(input);
	std::string output = listener.event_stream.str();

	bool ok = (output == expected_output);

	if (!ok) {
	    std::cerr <<
	    	__FILE__ << ':' << lineno << ": error: "
	    	"Failed scanner test \"" << test_name << "\" "
	    	"(" << output << " != " << expected_output << ")" << std::endl;
	}

	return ok;
}

bool test_scanner_with_input_single_bytes(const std::string& test_name, int lineno, const std::string& input, const std::string& expected_output) {
	test_scanner_listener listener;
	jsonxx::scanner s(listener);

	char buffer[] = { 0, 0, 0, 0, 0 };
	for (std::string::size_type i = 0; i < input.length(); ++i) {
		buffer[2] = input[i];
		s.scan(buffer + 2, buffer + 3);
	}
	std::string output = listener.event_stream.str();

	bool ok = (output == expected_output);

	if (!ok) {
	    std::cerr <<
	    	__FILE__ << ':' << lineno << ": error: "
	    	"Failed scanner test \"" << test_name << "\" "
	    	"(" << output << " != " << expected_output << ")" << std::endl;
	}

	return ok;
}

struct test_case {
	std::string test_name;
	int lineno;
	std::string input;
	std::string output;
};

#define CASE(name, input, expected_output) { name, __LINE__, input, expected_output }
test_case test_cases[] = {
	CASE("Simple object", "{}", "{}"),
	CASE("Ignores whitespace", " { \n} \r\n,", "{},"),
	CASE("All simple lexemes", "{}[]:, \n\r", "{}[]:,"),
	CASE("Empty string", "\"\"", "s()"),
	CASE("Empty string and stuff", "}\"\"{", "}s(){"),
	CASE("Simple string", "\"I am a string\"", "s(I am a string)"),
	CASE("Simple string and stuff", "}\"I am a string\"{", "}s(I am a string){"),
	CASE("Two simple strings", "\"a\",\"b\"", "s(a),s(b)"),
	CASE("String escape sequences", "\"test\\nescape\\t\\\"sequences\\\"\"", "s(test\nescape\t\"sequences\")"),
};

bool scanner_run_test_cases_big_chunk() {
	bool ok = true;
	for (size_t i = 0; i < (sizeof(test_cases) / sizeof(test_cases[0])); ++i) {
		const test_case& c = test_cases[i];
		ok &= test_scanner_with_input_one_chunk(c.test_name, c.lineno, c.input, c.output);
	}
	return ok;
}

bool scanner_run_test_cases_single_bytes() {
	bool ok = true;
	for (size_t i = 0; i < (sizeof(test_cases) / sizeof(test_cases[0])); ++i) {
		const test_case& c = test_cases[i];
		ok &= test_scanner_with_input_single_bytes(c.test_name, c.lineno, c.input, c.output);
	}
	return ok;
}

}

bool scanner_tests() {
    bool ok = true;

    ok &= EXEC(scanner_run_test_cases_big_chunk);
    ok &= EXEC(scanner_run_test_cases_single_bytes);

    return ok;
}
