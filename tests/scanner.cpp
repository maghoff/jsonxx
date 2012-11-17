#include <iostream>
#include <sstream>
#include "test_util.hpp"
#include "scanner.hpp"
#include "scanner_listener.hpp"

namespace {

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
	CASE("String unicode BMP escape sequence", "\"M\\u00e5ke\"", "s(M\xC3\xA5ke)"),
	CASE("String unicode outside of BMP escape sequence", "\"\\u20AC is a \\uD83D\\uDCA9 these days\"", "s(\xE2\x82\xAC is a \xF0\x9F\x92\xA9 these days)"),
	CASE("Invalid surrogate pair 1", "\"\\uD83D \\uDCA9\"", "X"),
	CASE("Invalid surrogate pair 2", "\"\\uDCA9\"", "X"),
	CASE("Invalid surrogate pair 3", "\"\\uD83D\\u20AC\"", "X"),
	CASE("Invalid surrogate pair 4", "\"\\uD83D\\t\"", "X"),
	CASE("Invalid surrogate pair 5", "\"\\uD83D\\ ...\"", "X"),
	CASE("True", "true", "t"),
	CASE("False", "false", "f"),
	CASE("Null", "null", "0"),
	CASE("True in context", "[true]", "[t]"),
	CASE("False in context", "[false]", "[f]"),
	CASE("Null in context", "[null]", "[0]"),
	CASE("Number -- integer", "[123]", "[n(123)]"),
	CASE("Number -- negative integer", "[-123]", "[n(-123)]"),
	CASE("Number -- fail on leading zero", "[0123]", "[X"),
	CASE("Number -- decimal", "[1.23]", "[n(1.23)]"),
	CASE("Number -- fail on lone period", "[12.]", "[X"),
	CASE("Number -- fail on lone period", "[12.e10]", "[X"),
	CASE("Number -- fail on lone period", "[.5]", "[X"),
	CASE("Number -- fail on lone e", "[12e]", "[X"),
	CASE("Number -- fail on lone e", "[e12]", "[X"),
	CASE("Number -- scientific plus", "[1.23e+10]", "[n(1.23e+10)]"),
	CASE("Number -- scientific minus", "[1.23e-10]", "[n(1.23e-10)]"),
	CASE("Number -- scientific small e", "[1.23e10]", "[n(1.23e10)]"),
	CASE("Number -- scientific capital E", "[1.23E10]", "[n(1.23E10)]"),
	CASE("Number -- integer with e", "[123E10]", "[n(123E10)]"),
	CASE("Syntax error on invalid top level input", "&", "X"),
	CASE("Syntax error stops the scanner", "{}&[]", "{}X"),
	CASE("Syntax error on illegal char in string", "\"lol\x14zomg\"", "X"),
	CASE("Syntax error on illegal escape sequence in string", "\"lol\\mcat\"", "X"),
	CASE("Syntax error on invalid char in the middle of a literal", "truse", "X"),
};


struct test_scanner_listener : jsonxx::scanner_listener {
	std::stringstream event_stream;

	void start_object() { event_stream << '{'; }
	void end_object() { event_stream << '}'; }

	void start_array() { event_stream << '['; }
	void end_array() { event_stream << ']'; }

	void comma() { event_stream << ','; }
	void colon() { event_stream << ':'; }

	void number(const std::string& n) { event_stream << "n(" << n << ")"; }
	void string(const std::string& s) { event_stream << "s(" << s << ")"; }
	void bool_true() { event_stream << 't'; }
	void bool_false() { event_stream << 'f'; }
	void null() { event_stream << '0'; }
};

bool test_scanner_with_input_one_chunk(const std::string& test_name, int lineno, const std::string& input, const std::string& expected_output) {
	test_scanner_listener listener;
	jsonxx::scanner s(listener);

	try {
		s.scan(input);
	}
	catch (const jsonxx::scanner::syntax_error&) {
		listener.event_stream << "X";
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

bool test_scanner_with_input_single_bytes(const std::string& test_name, int lineno, const std::string& input, const std::string& expected_output) {
	test_scanner_listener listener;
	jsonxx::scanner s(listener);

	char buffer[] = { 0, 0, 0, 0, 0 };
	for (std::string::size_type i = 0; i < input.length(); ++i) {
		buffer[2] = input[i];
		try {
			s.scan(buffer + 2, buffer + 3);
		}
		catch (const jsonxx::scanner::syntax_error&) {
			listener.event_stream << "X";
		}
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
