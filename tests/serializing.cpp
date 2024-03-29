#include <cassert>
#include <iostream>
#include <map>
#include <sstream>
#include <stdexcept>
#include "test_util.hpp"
#include "compact_writer.hpp"
#include "deserializer.hpp"
#include "parser.hpp"
#include "serializer.hpp"

namespace {

struct simple_struct {
    std::string name;
    int value;
};

jsonxx::type_information<simple_struct>::info_map_t create_info_map_for_simple_struct() {
    jsonxx::type_information<simple_struct>::info_map_t m;
    #define R(x) m[#x] = jsonxx::make_field_info(&simple_struct::x)
    R(name);
    R(value);
    #undef R
    return m;
}

}

template <>
jsonxx::type_information<simple_struct>::info_map_t
    jsonxx::type_information<simple_struct>::info_map = create_info_map_for_simple_struct();


namespace {

bool serialize_simple_struct() {
    bool ok = true;

    simple_struct s = { "the name", 42 };

    std::stringstream ss;
    jsonxx::compact_writer cw(ss);

    jsonxx::serializer(cw, s);

    CHECK_EQUAL(ss.str(), "{\"name\":\"the name\",\"value\":42}");

    return ok;
}

bool deserialize_simple_struct() {
    bool ok = true;

    simple_struct s = { "", 0 };

    std::stringstream ss("{\"name\":\"the name\",\"value\":42}");

    jsonxx::deserializer<simple_struct> ds(s);

    jsonxx::parser p(&ds);
    p.parse(ss);

    CHECK_EQUAL(s.name, "the name");
    CHECK_EQUAL(s.value, 42);

    return ok;
}

}

bool serializing_tests() {
    bool ok = true;

    ok &= EXEC(serialize_simple_struct);
    ok &= EXEC(deserialize_simple_struct);

    return ok;
}
