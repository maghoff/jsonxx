#include <iostream>
#include <sstream>
#include "compact_writer.hpp"


#define CHECK_EQUAL(a, b) \
    if ((a) != (b)) { \
        std::cerr << \
            __FILE__ << ':' << __LINE__ << ": " \
            "error: Failed test: " #a " == " #b " " \
            "(" << (a) << " != " << (b) << ')' << \
            std::endl; \
        ok = false; \
    }


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

bool execute(bool(*f)(), const char* f_name) {
    bool result = f();
    if (!result) {
        std::cerr << "Test failed: " << f_name << std::endl;
    }
    return result;
}

int main() {
    bool ok = true;

    #define EXEC(f) execute(f, #f)

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

    return ok ? 0 : 1;
}
