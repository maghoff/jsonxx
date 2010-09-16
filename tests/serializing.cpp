#include <iostream>
#include <sstream>
#include "test_util.hpp"
#include "compact_writer.hpp"

namespace {

struct simple_struct {
    std::string name;
    int value;
};

bool serialize_simple_struct() {
    bool ok = true;

    simple_struct s = { "the name", 42 };

    std::stringstream ss;
    jsonxx::compact_writer cw(ss);

    cw.start_object();
    cw.pair("name", s.name);
    cw.pair("value", s.value);
    cw.end_object();

    CHECK_EQUAL(ss.str(), "{\"name\":\"the name\",\"value\":42}");

    return ok;
}

}

bool serializing_tests() {
    bool ok = true;

    ok &= EXEC(serialize_simple_struct);

    return ok;
}


