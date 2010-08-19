#include <iostream>
#include <sstream>
#include "test_util.hpp"
#include "compact_writer.hpp"

namespace {

bool scoped_object() {
    bool ok = true;

    std::stringstream ss;
    jsonxx::compact_writer cw(ss);

    {
        jsonxx::scoped_object o(cw);
    }

    CHECK_EQUAL(ss.str(), "{}");

    return ok;
}

bool scoped_array() {
    bool ok = true;

    std::stringstream ss;
    jsonxx::compact_writer cw(ss);

    {
        jsonxx::scoped_object o(cw);
        cw.key("a");
        {
            jsonxx::scoped_array a(cw);
            cw.value(7);
        }
        cw.pair("b", 42);
    }

    CHECK_EQUAL(ss.str(), "{\"a\":[7],\"b\":42}");

    return ok;
}

}

bool raii_tests() {
    bool ok = true;

    ok &= EXEC(scoped_object);
    ok &= EXEC(scoped_array);

    return ok;
}

