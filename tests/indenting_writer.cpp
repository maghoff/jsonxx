#include <sstream>
#include "indenting_writer.hpp"
#include "test_util.hpp"

namespace {

bool trivial() {
    bool ok = true;

    std::stringstream ss;
    jsonxx::indenting_writer cw(ss);

    cw.start_object();
    cw.end_object();

    CHECK_EQUAL(ss.str(), "{\n}");

    return ok;
}

bool simple() {
    bool ok = true;

    std::stringstream ss;
    jsonxx::indenting_writer cw(ss);

    cw.start_object();
    cw.pair("i", 7);
    cw.end_object();

    CHECK_EQUAL(ss.str(), "{\n    \"i\": 7\n}");

    return ok;
}

bool several() {
    bool ok = true;

    std::stringstream ss;
    jsonxx::indenting_writer cw(ss);

    cw.start_object();
    cw.pair("i", 7);
    cw.pair("j", 2.72);
    cw.pair("k", "v");
    cw.end_object();

    CHECK_EQUAL(ss.str(),
        "{\n"
        "    \"i\": 7,\n"
        "    \"j\": 2.72,\n"
        "    \"k\": \"v\"\n"
        "}"
    );

    return ok;
}

bool list() {
    bool ok = true;

    std::stringstream ss;
    jsonxx::indenting_writer cw(ss);

    cw.start_object();
    cw.key("l");
    cw.start_array();
    cw.value(7);
    cw.value(2.72);
    cw.value("v");
    cw.end_array();
    cw.end_object();

    CHECK_EQUAL(ss.str(),
        "{\n"
        "    \"l\": [\n"
        "        7,\n"
        "        2.72,\n"
        "        \"v\"\n"
        "    ]\n"
        "}"
    );

    return ok;
}

bool complex_nesting_all_types() {
    bool ok = true;

    std::stringstream ss;
    jsonxx::indenting_writer cw(ss);

    cw.start_object();
    cw.key("a");
    cw.start_array();
    cw.value(1);
    cw.value(1.5);
    cw.value_bool(true);
    cw.start_object();
        cw.key("a");
        cw.start_array();
        cw.value(jsonxx::null);
        cw.value("str");
        cw.value(1.4);
        cw.end_array();
        cw.key("c");
        cw.start_array();
        cw.value_bool(false);
        cw.value(21);
        cw.value(std::string("std"));
        cw.end_array();
    cw.end_object();
    cw.value(0.7);
    cw.value(89);
    cw.value(jsonxx::null);
    cw.end_array();
    cw.end_object();

    CHECK_EQUAL(ss.str(),
        "{\n"
        "    \"a\": [\n"
        "        1,\n"
        "        1.5,\n"
        "        true,\n"
        "        {\n"
        "            \"a\": [\n"
        "                null,\n"
        "                \"str\",\n"
        "                1.4\n"
        "            ],\n"
        "            \"c\": [\n"
        "                false,\n"
        "                21,\n"
        "                \"std\"\n"
        "            ]\n"
        "        },\n"
        "        0.7,\n"
        "        89,\n"
        "        null\n"
        "    ]\n"
        "}"
    );

    return ok;
}

void function_overload_ref(jsonxx::object_listener& out) {
    out.start_object();
    out.pair("hello", "ref");
    out.end_object();
}

void function_overload_ptr(jsonxx::object_listener& out) {
    out.start_object();
    out.pair("hello", "ptr");
    out.end_object();
}

bool function_overload() {
    bool ok = true;

    std::stringstream ss;
    jsonxx::indenting_writer cw(ss);
    cw.start_object();
    cw.pair("ref", function_overload_ref);
    cw.pair("ptr", function_overload_ptr);
    cw.end_object();

    return ok;
}

bool nesting_empty_object() {
    bool ok = true;

    std::stringstream ss;
    jsonxx::indenting_writer cw(ss);

    cw.start_object();
    cw.key("a");
    cw.start_object();
    cw.end_object();
    cw.pair("b", 5);
    cw.end_object();

    CHECK_EQUAL(ss.str(),
        "{\n"
        "    \"a\": {\n"
        "    },\n"
        "    \"b\": 5\n"
        "}"
    );

    return ok;
}

bool nesting_empty_array() {
    bool ok = true;

    std::stringstream ss;
    jsonxx::indenting_writer cw(ss);

    cw.start_object();
    cw.key("a");
    cw.start_array();
    cw.end_array();
    cw.pair("b", 5);
    cw.end_object();

    CHECK_EQUAL(ss.str(),
        "{\n"
        "    \"a\": [\n"
        "    ],\n"
        "    \"b\": 5\n"
        "}"
    );

    return ok;
}

bool nesting_empty_array_in_array() {
    bool ok = true;

    std::stringstream ss;
    jsonxx::indenting_writer cw(ss);

    cw.start_object();
    cw.key("a");
    cw.start_array();
    cw.start_array();
    cw.end_array();
    cw.value(5);
    cw.end_array();
    cw.end_object();

    CHECK_EQUAL(ss.str(),
        "{\n"
        "    \"a\": [\n"
        "        [\n"
        "        ],\n"
        "        5\n"
        "    ]\n"
        "}"
    );

    return ok;
}

}

bool indenting_writer_tests() {
    bool ok = true;

    #define E(f) execute(f, "indenting_writer::" #f)

    ok &= E(trivial);
    ok &= E(simple);
    ok &= E(several);
    ok &= E(list);
    ok &= E(complex_nesting_all_types);
    ok &= E(function_overload);
    ok &= E(nesting_empty_object);
    ok &= E(nesting_empty_array);
    ok &= E(nesting_empty_array_in_array);

    return ok;
}
