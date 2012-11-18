#include <iostream>
#include <sstream>
#include "compact_writer.hpp"
#include "parser2.hpp"
#include "scanner.hpp"
#include "test_util.hpp"
#include "validating_filter.hpp"

namespace {

JSONXX_DEPRECATED("Uses deprecated functionality parser2::lol_get_scanner_lisener")
std::string roundtrip(const std::string& in) {
	std::stringstream ss;
	try {
		jsonxx::compact_writer cw(ss);
		jsonxx::validating_filter vf(&cw);
		jsonxx::parser2 p(vf);
		jsonxx::scanner s(p.lol_get_scanner_listener());
		s.scan(in);
	}
	catch (std::exception& x) {
		ss << 'X';
	}
	return ss.str();
}

JSONXX_DEPRECATED("Depends on deprecated roundtrip")
bool empty() {
	bool ok = true;

	std::string res = roundtrip("{ }");
	CHECK_EQUAL(res, "{}");

	return ok;
}

JSONXX_DEPRECATED("Depends on deprecated roundtrip")
bool key_int() {
	bool ok = true;

	std::string res = roundtrip("{ \"i\": 5 }");
	CHECK_EQUAL(res, "{\"i\":5}");

	return ok;
}

JSONXX_DEPRECATED("Depends on deprecated roundtrip")
bool key_float() {
	bool ok = true;

	std::string res = roundtrip("{ \"f\": 5.5 }");
	CHECK_EQUAL(res, "{\"f\":5.5}");

	return ok;
}

JSONXX_DEPRECATED("Depends on deprecated roundtrip")
bool key_null() {
	bool ok = true;

	std::string res = roundtrip("{ \"n\": null }");
	CHECK_EQUAL(res, "{\"n\":null}");

	return ok;
}

JSONXX_DEPRECATED("Depends on deprecated roundtrip")
bool boolean_true() {
	bool ok = true;

	std::string res = roundtrip("{ \"t\": true }");
	CHECK_EQUAL(res, "{\"t\":true}");

	return ok;
}

JSONXX_DEPRECATED("Depends on deprecated roundtrip")
bool boolean_false() {
	bool ok = true;

	std::string res = roundtrip("{ \"f\": false }");
	CHECK_EQUAL(res, "{\"f\":false}");

	return ok;
}

JSONXX_DEPRECATED("Depends on deprecated roundtrip")
bool object_several_members() {
	bool ok = true;

	std::string res = roundtrip("{ \"a\": false , \"b\": false, \"c\": false }");
	CHECK_EQUAL(res, "{\"a\":false,\"b\":false,\"c\":false}");

	return ok;
}

JSONXX_DEPRECATED("Depends on deprecated roundtrip")
bool object_nested() {
	bool ok = true;

	std::string res = roundtrip("{ \"a\": false, \"b\": { \"d\" :false }, \"c\": false }");
	CHECK_EQUAL(res, "{\"a\":false,\"b\":{\"d\":false},\"c\":false}");

	return ok;
}

JSONXX_DEPRECATED("Depends on deprecated roundtrip")
bool simple_string() {
	bool ok = true;

	std::string res = roundtrip("{ \"s\": \"Simple text string\" }");
	CHECK_EQUAL(res, "{\"s\":\"Simple text string\"}");

	return ok;
}

JSONXX_DEPRECATED("Depends on deprecated roundtrip")
bool string_with_escapes() {
	bool ok = true;

	std::string s =
		"{\"s\":"
		"\"Multiline\\n\\ttext\\r\\nwith bells\\b and whistles\\f\\n"
		"Quotes: \\\"\\n"
		"Backslash: \\\\\\n"
		"\"}"
	;

	std::string res = roundtrip(s);
	CHECK_EQUAL(res, s);

	return ok;
}

JSONXX_DEPRECATED("Depends on deprecated roundtrip")
bool utf8_string() {
	bool ok = true;

	std::string res = roundtrip("{ \"s\": \"Please pay in \xE2\x82\xAC\" }");
	CHECK_EQUAL(res, "{\"s\":\"Please pay in \xE2\x82\xAC\"}");

	return ok;
}

JSONXX_DEPRECATED("Depends on deprecated roundtrip")
bool string_with_unicode_escapes() {
	bool ok = true;

	std::string res = roundtrip("{ \"s\": \"Please pay in \\u20AC\" }");
	CHECK_EQUAL(res, "{\"s\":\"Please pay in \xE2\x82\xAC\"}");

	return ok;
}

JSONXX_DEPRECATED("Depends on deprecated parser2::lol_get_scanner_listener")
bool incremental() {
	bool ok = true;

	std::stringstream ss;
	jsonxx::compact_writer cw(ss);
	jsonxx::validating_filter vf(&cw);
	jsonxx::parser2 p(vf);
	jsonxx::scanner s(p.lol_get_scanner_listener());

	s.scan("{");
	CHECK_EQUAL(ss.str(), "{");
	CHECK_EQUAL(p.is_done(), false);

	s.scan("\"t\":");
	CHECK_EQUAL(ss.str(), "{\"t\":");
	CHECK_EQUAL(p.is_done(), false);

	s.scan("true,");
	CHECK_EQUAL(ss.str(), "{\"t\":true");
	CHECK_EQUAL(p.is_done(), false);

	s.scan("\"f\"");
	CHECK_EQUAL(ss.str(), "{\"t\":true,\"f\":");
	CHECK_EQUAL(p.is_done(), false);

	s.scan(":fals");
	CHECK_EQUAL(ss.str(), "{\"t\":true,\"f\":");
	CHECK_EQUAL(p.is_done(), false);

	s.scan("e");
	CHECK_EQUAL(ss.str(), "{\"t\":true,\"f\":false");
	CHECK_EQUAL(p.is_done(), false);

	s.scan("}");
	CHECK_EQUAL(ss.str(), "{\"t\":true,\"f\":false}");
	CHECK_EQUAL(p.is_done(), true);

	return ok;
}

JSONXX_DEPRECATED("Depends on deprecated roundtrip")
bool array() {
	bool ok = true;

	std::string res = roundtrip("{ \"a\": [1, 2, 3] }");
	CHECK_EQUAL(res, "{\"a\":[1,2,3]}");

	return ok;
}

JSONXX_DEPRECATED("Depends on deprecated roundtrip")
bool value_stream() {
	bool ok = true;

	std::stringstream ss;
	jsonxx::compact_writer cw(ss);
	jsonxx::parser2 p(cw, &jsonxx::parser2_state::expect_value_stream);
	jsonxx::scanner s(p.lol_get_scanner_listener());

	s.scan("{ \"a\": 1 }{ \"b\": 2 }");
	CHECK_EQUAL(ss.str(), "{\"a\":1},{\"b\":2}");

	return ok;
}

}

JSONXX_DEPRECATED("Indirectly depends on deprecated roundtrip")
bool parser2_tests() {
	bool ok = true;

	ok &= EXEC(empty);
	ok &= EXEC(key_int);
	ok &= EXEC(key_float);
	ok &= EXEC(key_null);
	ok &= EXEC(boolean_true);
	ok &= EXEC(boolean_false);
	ok &= EXEC(object_several_members);
	ok &= EXEC(object_nested);
	ok &= EXEC(simple_string);
	ok &= EXEC(string_with_escapes);
	ok &= EXEC(utf8_string);
	ok &= EXEC(string_with_unicode_escapes);
	ok &= EXEC(incremental);
	ok &= EXEC(array);
	ok &= EXEC(value_stream);

	return ok;
}
