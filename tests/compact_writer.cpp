#include <iostream>
#include <sstream>
#include "test_util.hpp"
#include "compact_writer.hpp"

namespace {

bool compact_writer_simple() {
    bool ok = true;

    std::stringstream ss;
    jsonxx::compact_writer cw(ss);

    cw.start_object();
    cw.end_object();

    CHECK_EQUAL(ss.str(), "{}");

    return ok;
}

bool compact_writer_int() {
    bool ok = true;

    std::stringstream ss;
    jsonxx::compact_writer cw(ss);

    cw.start_object();
    cw.key("i");
    cw.value(7);
    cw.end_object();

    CHECK_EQUAL(ss.str(), "{\"i\":7}");

    return ok;
}

bool compact_writer_double() {
    bool ok = true;

    std::stringstream ss;
    jsonxx::compact_writer cw(ss);

    cw.start_object();
    cw.key("d");
    cw.value(3.14159265359);
    cw.end_object();

    CHECK_EQUAL(ss.str(), "{\"d\":3.14159}");

    return ok;
}

bool compact_writer_bool() {
    bool ok = true;

    std::stringstream ss;
    jsonxx::compact_writer cw(ss);

    cw.start_object();
    cw.key("b");
    cw.value_bool(true);
    cw.end_object();

    CHECK_EQUAL(ss.str(), "{\"b\":true}");

    return ok;
}

bool compact_writer_c_string() {
    bool ok = true;

    std::stringstream ss;
    jsonxx::compact_writer cw(ss);

    cw.start_object();
    cw.key("c");
    cw.value("c-string");
    cw.end_object();

    CHECK_EQUAL(ss.str(), "{\"c\":\"c-string\"}");

    return ok;
}

bool compact_writer_std_string() {
    bool ok = true;

    std::stringstream ss;
    jsonxx::compact_writer cw(ss);

    cw.start_object();
    cw.key("s");
    std::string str("std-string");
    cw.value(str);
    cw.end_object();

    CHECK_EQUAL(ss.str(), "{\"s\":\"std-string\"}");

    return ok;
}

bool compact_writer_null() {
    bool ok = true;

    std::stringstream ss;
    jsonxx::compact_writer cw(ss);

    cw.start_object();
    cw.key("n");
    cw.value(jsonxx::null);
    cw.end_object();

    CHECK_EQUAL(ss.str(), "{\"n\":null}");

    return ok;
}

bool compact_writer_all_basic_types() {
    bool ok = true;

    std::stringstream ss;
    jsonxx::compact_writer cw(ss);

    cw.start_object();
    cw.key("i"); cw.value(7);
    cw.key("d"); cw.value(3.14159265359);
    cw.key("b"); cw.value_bool(true);
    cw.key("c"); cw.value("c-string");
    cw.key("s"); cw.value(std::string("std-string"));
    cw.key("n"); cw.value(jsonxx::null);
    cw.end_object();

    CHECK_EQUAL(ss.str(),
        "{"
            "\"i\":7,"
            "\"d\":3.14159,"
            "\"b\":true,"
            "\"c\":\"c-string\","
            "\"s\":\"std-string\","
            "\"n\":null"
        "}");

    return ok;
}

bool compact_writer_double_high_precision() {
    bool ok = true;

    std::stringstream ss;
    ss.precision(12);
    jsonxx::compact_writer cw(ss);

    cw.start_object();
    cw.key("d");
    cw.value(3.14159265359);
    cw.end_object();

    CHECK_EQUAL(ss.str(), "{\"d\":3.14159265359}");

    return ok;
}

bool compact_writer_double_fixed_precision() {
    bool ok = true;

    std::stringstream ss;
    ss.setf(std::ios::fixed, std::ios::floatfield);
    ss.precision(3);
    jsonxx::compact_writer cw(ss);

    cw.start_object();
    cw.key("d1");
    cw.value(3.14159265359);
    cw.key("d2");
    cw.value(20.0);
    cw.end_object();

    CHECK_EQUAL(ss.str(), "{\"d1\":3.142,\"d2\":20.000}");

    return ok;
}

bool compact_writer_nested_objects() {
    bool ok = true;

    std::stringstream ss;
    jsonxx::compact_writer cw(ss);

    cw.start_object();
    cw.key("i1");
    cw.value(1);
    cw.key("o");
    cw.start_object();
    cw.end_object();
    cw.end_object();

    CHECK_EQUAL(ss.str(), "{\"i1\":1,\"o\":{}}");

    return ok;
}

bool compact_writer_nested_objects_complex() {
    bool ok = true;

    std::stringstream ss;
    jsonxx::compact_writer cw(ss);

    cw.start_object();
        cw.key("o1");
        cw.start_object();
            cw.key("i");
            cw.value(1);
            cw.key("o");
            cw.start_object();
                cw.key("i");
                cw.value(1);
            cw.end_object();
        cw.end_object();
        cw.key("i1");
        cw.value(7);
        cw.key("o2");
        cw.start_object();
            cw.key("i");
            cw.value(1);
            cw.key("o");
            cw.start_object();
                cw.key("i");
                cw.value(1);
            cw.end_object();
        cw.end_object();
        cw.key("i2");
        cw.value(42);
        cw.key("i3");
        cw.value(1337);
    cw.end_object();

    CHECK_EQUAL(ss.str(),
        "{"
            "\"o1\":{"
                "\"i\":1,"
                "\"o\":{"
                    "\"i\":1"
                "}"
            "},"
            "\"i1\":7,"
            "\"o2\":{"
                "\"i\":1,"
                "\"o\":{"
                    "\"i\":1"
                "}"
            "},"
            "\"i2\":42,"
            "\"i3\":1337"
        "}"
    );

    return ok;
}

bool compact_writer_empty_array() {
    bool ok = true;

    std::stringstream ss;
    jsonxx::compact_writer cw(ss);

    cw.start_object();
    cw.key("a");
    cw.start_array();
    cw.end_array();
    cw.end_object();

    CHECK_EQUAL(ss.str(), "{\"a\":[]}");

    return ok;
}

bool compact_writer_simple_array() {
    bool ok = true;

    std::stringstream ss;
    jsonxx::compact_writer cw(ss);

    cw.start_object();
    cw.key("a");
    cw.start_array();
    cw.value(5);
    cw.end_array();
    cw.end_object();

    CHECK_EQUAL(ss.str(), "{\"a\":[5]}");

    return ok;
}

bool compact_writer_array() {
    bool ok = true;

    std::stringstream ss;
    jsonxx::compact_writer cw(ss);

    cw.start_object();
    cw.key("a");
    cw.start_array();
    cw.value(1);
    cw.value(1);
    cw.value(2);
    cw.end_array();
    cw.key("b");
    cw.value(3);
    cw.key("c");
    cw.start_array();
    cw.value(5);
    cw.value(8);
    cw.value(13);
    cw.end_array();
    cw.end_object();

    CHECK_EQUAL(ss.str(), "{\"a\":[1,1,2],\"b\":3,\"c\":[5,8,13]}");

    return ok;
}

bool compact_writer_complex_nesting() {
    bool ok = true;

    std::stringstream ss;
    jsonxx::compact_writer cw(ss);

    cw.start_object();
    cw.key("a");
    cw.start_array();
    cw.value(1);
    cw.value(1);
    cw.value(2);
    cw.start_object();
        cw.key("a");
        cw.start_array();
        cw.value(3);
        cw.value(5);
        cw.value(8);
        cw.end_array();
        cw.key("c");
        cw.start_array();
        cw.value(13);
        cw.value(21);
        cw.value(34);
        cw.end_array();
    cw.end_object();
    cw.value(55);
    cw.value(89);
    cw.value(144);
    cw.end_array();
    cw.end_object();

    CHECK_EQUAL(ss.str(), "{\"a\":[1,1,2,{\"a\":[3,5,8],\"c\":[13,21,34]},55,89,144]}");

    return ok;
}

bool compact_writer_complex_nesting_all_types() {
    bool ok = true;

    std::stringstream ss;
    jsonxx::compact_writer cw(ss);

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

    CHECK_EQUAL(ss.str(), "{\"a\":[1,1.5,true,{\"a\":[null,\"str\",1.4],\"c\":[false,21,\"std\"]},0.7,89,null]}");

    return ok;
}

}

bool compact_writer_tests() {
    bool ok = true;

    ok &= EXEC(compact_writer_simple);
    ok &= EXEC(compact_writer_int);
    ok &= EXEC(compact_writer_double);
    ok &= EXEC(compact_writer_bool);
    ok &= EXEC(compact_writer_c_string);
    ok &= EXEC(compact_writer_std_string);
    ok &= EXEC(compact_writer_null);
    ok &= EXEC(compact_writer_all_basic_types);
    ok &= EXEC(compact_writer_double_high_precision);
    ok &= EXEC(compact_writer_double_fixed_precision);
    ok &= EXEC(compact_writer_nested_objects);
    ok &= EXEC(compact_writer_nested_objects_complex);
    ok &= EXEC(compact_writer_empty_array);
    ok &= EXEC(compact_writer_simple_array);
    ok &= EXEC(compact_writer_array);
    ok &= EXEC(compact_writer_complex_nesting);
    ok &= EXEC(compact_writer_complex_nesting_all_types);

    return ok;
}
