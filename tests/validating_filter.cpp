#include <iostream>
#include "test_util.hpp"
#include "null_object_listener.hpp"
#include "validating_filter.hpp"

namespace {

bool validating_filter_simple() {
    jsonxx::null_object_listener null;
    jsonxx::validating_filter v(&null);

    try {
        v.end_object();
    }
    catch(const jsonxx::validation_error&) {
        return true;
    }

    REPORT("Control should have been diverted by an exception");
    return false;
}

}

bool validating_filter_tests() {
    bool ok = true;

    ok &= EXEC(validating_filter_simple);

    return ok;
}
