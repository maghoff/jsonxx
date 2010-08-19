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
    cw.key("i");
    cw.value(7);
    cw.end_object();

    CHECK_EQUAL(ss.str(), "{\n    \"i\":7\n}");

    return ok;
}

}

bool indenting_writer_tests() {
    bool ok = true;

    #define E(f) execute(f, "indenting_writer::" #f)

    ok &= E(trivial);
//    ok &= E(simple);

    return ok;
}
