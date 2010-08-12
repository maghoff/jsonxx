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

    return ok ? 0 : 1;
}
