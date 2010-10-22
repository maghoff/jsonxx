#include <iostream>
#include <sstream>
#include "compact_writer.hpp"
#include "parser.hpp"
#include "test_util.hpp"
#include "validating_filter.hpp"

namespace {

std::string roundtrip(const std::string& in) {
    std::stringstream ss;
    {
        jsonxx::compact_writer cw(ss);
        jsonxx::validating_filter vf(&cw);
        jsonxx::parser p(&vf);

        p.parse(in);
    }
    return ss.str();
}

bool empty() {
    bool ok = true;

    std::string res = roundtrip("{ }");
    CHECK_EQUAL(res, "{}");

    return ok;
}

bool key_int() {
    bool ok = true;

    std::string res = roundtrip("{ \"i\": 5 }");
    CHECK_EQUAL(res, "{\"i\":5}");

    return ok;
}

bool key_float() {
    bool ok = true;

    std::string res = roundtrip("{ \"f\": 5.5 }");
    CHECK_EQUAL(res, "{\"f\":5.5}");

    return ok;
}

bool key_null() {
    bool ok = true;

    std::string res = roundtrip("{ \"n\": null }");
    CHECK_EQUAL(res, "{\"n\":null}");

    return ok;
}

bool booleans() {
    bool ok = true;

    std::string res = roundtrip("{ \"t\": true, \"f\": false }");
    CHECK_EQUAL(res, "{\"t\":true,\"f\":false}");

    return ok;
}

bool simple_string() {
    bool ok = true;

    std::string res = roundtrip("{ \"s\": \"Simple text string\" }");
    CHECK_EQUAL(res, "{\"s\":\"Simple text string\"}");

    return ok;
}

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

bool utf8_string() {
    bool ok = true;

    std::string res = roundtrip("{ \"s\": \"Please pay in \xE2\x82\xAC\" }");
    CHECK_EQUAL(res, "{\"s\":\"Please pay in \xE2\x82\xAC\"}");

    return ok;
}

bool string_with_unicode_escapes() {
    bool ok = true;

    std::string res = roundtrip("{ \"s\": \"Please pay in \\u20AC\" }");
    CHECK_EQUAL(res, "{\"s\":\"Please pay in \xE2\x82\xAC\"}");

    return ok;
}

bool incremental() {
    bool ok = true;

    std::stringstream ss;
    jsonxx::compact_writer cw(ss);
    jsonxx::validating_filter vf(&cw);
    jsonxx::parser p(&vf);

    p.parse("{");
    CHECK_EQUAL(ss.str(), "{");
    CHECK_EQUAL(p.is_done(), false);

    p.parse("\"t\":");
    CHECK_EQUAL(ss.str(), "{\"t\":");
    CHECK_EQUAL(p.is_done(), false);

    p.parse("true,");
    CHECK_EQUAL(ss.str(), "{\"t\":true");
    CHECK_EQUAL(p.is_done(), false);

    p.parse("\"f\":fals");
    CHECK_EQUAL(ss.str(), "{\"t\":true,\"f\":");
    CHECK_EQUAL(p.is_done(), false);

    p.parse("e}");
    CHECK_EQUAL(ss.str(), "{\"t\":true,\"f\":false}");
    CHECK_EQUAL(p.is_done(), true);

    return ok;
}

bool array() {
    bool ok = true;

    std::string res = roundtrip("{ \"a\": [1, 2, 3] }");
    CHECK_EQUAL(res, "{\"a\":[1,2,3]}");

    return ok;
}

}

bool parser_tests() {
    bool ok = true;

    ok &= EXEC(empty);
    ok &= EXEC(key_int);
    ok &= EXEC(key_float);
    ok &= EXEC(key_null);
    ok &= EXEC(booleans);
    ok &= EXEC(simple_string);
    ok &= EXEC(string_with_escapes);
    ok &= EXEC(utf8_string);
    ok &= EXEC(string_with_unicode_escapes);
    ok &= EXEC(incremental);
    ok &= EXEC(array);

    return ok;
}
