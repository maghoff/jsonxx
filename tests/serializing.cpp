#include <cassert>
#include <iostream>
#include <map>
#include <sstream>
#include <stdexcept>
#include "test_util.hpp"
#include "compact_writer.hpp"
#include "deserializer.hpp"
#include "parser.hpp"

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

typedef jsonxx::deserializer<simple_struct> simple_struct_deserializer;

template <class T>
void simple_serializer(jsonxx::object_listener& o, const T& t) {
    typename jsonxx::type_information<T>::info_map_t &info_map = jsonxx::type_information<T>::info_map;
    typedef typename jsonxx::type_information<T>::info_map_t::const_iterator c_iter;
    c_iter begin = info_map.begin(), end = info_map.end();

    o.start_object();
    for (c_iter i = begin; i != end; ++i) {
        o.key(i->first);
        if (i->second.type == jsonxx::field_type::t_string) o.value(t.*i->second.field.t_string);
        else if (i->second.type == jsonxx::field_type::t_int) o.value(t.*i->second.field.t_int);
        else if (i->second.type == jsonxx::field_type::t_double) o.value(t.*i->second.field.t_double);
        else if (i->second.type == jsonxx::field_type::t_bool) o.value(t.*i->second.field.t_bool);
        else {
            assert(false);
            o.value(jsonxx::null);
        }
    }
    o.end_object();
}

bool serialize_simple_struct() {
    bool ok = true;

    simple_struct s = { "the name", 42 };

    std::stringstream ss;
    jsonxx::compact_writer cw(ss);

    simple_serializer(cw, s);

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
