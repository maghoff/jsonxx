#include <cassert>
#include <iostream>
#include <map>
#include <sstream>
#include <stdexcept>
#include "test_util.hpp"
#include "compact_writer.hpp"
#include "deserializer_base.hpp"
#include "parser.hpp"

namespace {

struct simple_struct {
    std::string name;
    int value;
};

class simple_struct_deserializer : public jsonxx::deserializer_base<simple_struct> {
public:
    simple_struct_deserializer(simple_struct&);
    ~simple_struct_deserializer();
};

simple_struct_deserializer::simple_struct_deserializer(simple_struct& s) :
    jsonxx::deserializer_base<simple_struct>(s)
{
    REGISTER_FIELD(name);
    REGISTER_FIELD(value);
}

simple_struct_deserializer::~simple_struct_deserializer() {
}

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

bool deserialize_simple_struct() {
    bool ok = true;

    simple_struct s = { "", 0 };

    std::stringstream ss("{\"name\":\"the name\",\"value\":42}");
    simple_struct_deserializer ds(s);
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
