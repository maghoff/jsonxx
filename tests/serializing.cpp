#include <cassert>
#include <iostream>
#include <map>
#include <sstream>
#include <stdexcept>
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

    union field_pointer {
        std::string simple_struct::* t_string;
        int simple_struct::* t_int;
        double simple_struct::* t_double;
        bool simple_struct::* t_bool;
    };

    enum field_type {
        t_ignore,
        t_string,
        t_int,
        t_double,
        t_bool
    };

    struct field_info {
        field_pointer field;
        field_type type;
    };

    field_info expected_field;

    std::map<std::string, field_info> expect_from_key;

    void match(field_type e);

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
    #define REGISTER(f, t) \
        expect_from_key[#f].field.t = &simple_struct::f; \
        expect_from_key[#f].type = t;

    REGISTER(name, t_string);
    REGISTER(value, t_int);

    #undef REGISTER
}

simple_struct_object_listener::~simple_struct_object_listener() {
}

void simple_struct_object_listener::key(const std::string& k) {
    expected_field = expect_from_key[k];
}

void simple_struct_object_listener::start_object() { }
void simple_struct_object_listener::end_object() { }

void simple_struct_object_listener::start_array() { }
void simple_struct_object_listener::end_array() { }

void simple_struct_object_listener::match(field_type e) {
    if (expected_field.type != e) {
        throw std::runtime_error("Actual type did not match expected type when deserializing");
    }
}

void simple_struct_object_listener::value(const std::string& v) {
    match(t_string);
    &s->*expected_field.field.t_string = v;
}

void simple_struct_object_listener::value(int v) {
    match(t_int);
    &s->*expected_field.field.t_int = v;
}

void simple_struct_object_listener::value(double v) {
    match(t_double);
    &s->*expected_field.field.t_double = v;
}

void simple_struct_object_listener::value(jsonxx::bool_type v) {
    match(t_bool);
    &s->*expected_field.field.t_bool = v.value;
}

void simple_struct_object_listener::value(jsonxx::null_type) {
    assert(false);
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
