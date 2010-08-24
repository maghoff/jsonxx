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

bool incremental() {
    bool ok = true;

    std::stringstream ss;
    jsonxx::compact_writer cw(ss);
    jsonxx::validating_filter vf(&cw);
    jsonxx::parser p(&vf);

    p.parse("{");
    CHECK_EQUAL(ss.str(), "{");

    p.parse("\"t\":");
    CHECK_EQUAL(ss.str(), "{\"t\":");

    p.parse("true,");
    CHECK_EQUAL(ss.str(), "{\"t\":true");

    p.parse("\"f\":fals");
    CHECK_EQUAL(ss.str(), "{\"t\":true,\"f\":");

    p.parse("e}");
    CHECK_EQUAL(ss.str(), "{\"t\":true,\"f\":false}");

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
    ok &= EXEC(incremental);

    return ok;
}


