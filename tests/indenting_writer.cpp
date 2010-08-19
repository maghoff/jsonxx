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

}

bool indenting_writer_tests() {
    bool ok = true;

    #define E(f) execute(f, "indenting_writer::" #f)

    ok &= E(trivial);
    ok &= E(simple);
    ok &= E(several);
    ok &= E(list);

    return ok;
}
