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

template <class T>
class deserializer_base : public jsonxx::object_listener {
protected:
    typedef T target_struct_type;
    T& t;

    union field_pointer {
        std::string T::* t_string;
        int T::* t_int;
        double T::* t_double;
        bool T::* t_bool;
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

        field_info() { }
        field_info(std::string T::* p) : type(t_string) { field.t_string = p; }
        field_info(int T::* p) : type(t_int) { field.t_int = p; }
        field_info(double T::* p) : type(t_double) { field.t_double = p; }
        field_info(bool T::* p) : type(t_bool) { field.t_bool = p; }
    };

    field_info expected_field;

    typedef std::map<std::string, field_info> expect_map_t;
    expect_map_t expect_from_key;

public:
    deserializer_base(T&);
    ~deserializer_base();

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

#define REGISTER_FIELD(f) \
    expect_from_key[#f] = field_info(&target_struct_type::f);

template <class T>
deserializer_base<T>::deserializer_base(T& t_) :
    t(t_)
{
}

template <class T>
deserializer_base<T>::~deserializer_base() {
}

template <class T>
void deserializer_base<T>::key(const std::string& k) {
    typename expect_map_t::const_iterator i = expect_from_key.find(k);
    if (i != expect_from_key.end()) expected_field = i->second;
    else expected_field.type = t_ignore;
}

template <class T>
void deserializer_base<T>::start_object() { }
template <class T>
void deserializer_base<T>::end_object() { }

template <class T>
void deserializer_base<T>::start_array() { }
template <class T>
void deserializer_base<T>::end_array() { }

template <class T>
void deserializer_base<T>::value(const std::string& v) {
    if (expected_field.type == t_string) &t->*expected_field.field.t_string = v;
}

template <class T>
void deserializer_base<T>::value(int v) {
    if (expected_field.type == t_int) &t->*expected_field.field.t_int = v;
}

template <class T>
void deserializer_base<T>::value(double v) {
    if (expected_field.type == t_double) &t->*expected_field.field.t_double = v;
}

template <class T>
void deserializer_base<T>::value(jsonxx::bool_type v) {
    if (expected_field.type == t_bool) &t->*expected_field.field.t_bool = v.value;
}

template <class T>
void deserializer_base<T>::value(jsonxx::null_type) {
    assert(false);
}

class simple_struct_deserializer : public deserializer_base<simple_struct> {
public:
    simple_struct_deserializer(simple_struct&);
    ~simple_struct_deserializer();
};

simple_struct_deserializer::simple_struct_deserializer(simple_struct& s) :
    deserializer_base<simple_struct>(s)
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
