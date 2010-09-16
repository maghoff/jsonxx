#include <iostream>
#include <sstream>
#include "test_util.hpp"
#include "compact_writer.hpp"
#include "parser.hpp"

namespace {

struct simple_struct {
    std::string name;
    int value;
};

class simple_struct_object_listener : public jsonxx::object_listener {
    simple_struct& s;

public:
    simple_struct_object_listener(simple_struct&);
    ~simple_struct_object_listener();

    void key(const std::string&);

    void start_object();
    void end_object();

    void start_array();
    void end_array();

    void value(const std::string&);
    void value(int);
    void value(double);
    void value(jsonxx::bool_type);
    void value(jsonxx::null_type);
};

simple_struct_object_listener::simple_struct_object_listener(simple_struct& s_) :
    s(s_)
{

}

simple_struct_object_listener::~simple_struct_object_listener() {

}

void simple_struct_object_listener::key(const std::string&) {

}

void simple_struct_object_listener::start_object() { }
void simple_struct_object_listener::end_object() { }

void simple_struct_object_listener::start_array() { }
void simple_struct_object_listener::end_array() { }

void simple_struct_object_listener::value(const std::string& v) { s.name = v; }
void simple_struct_object_listener::value(int v) { s.value = v; }
void simple_struct_object_listener::value(double) { }
void simple_struct_object_listener::value(jsonxx::bool_type) { }
void simple_struct_object_listener::value(jsonxx::null_type) { }

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
    simple_struct_object_listener ds(s);
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


