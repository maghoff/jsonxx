#ifndef JSONXX_DESERIALIZER_HPP
#define JSONXX_DESERIALIZER_HPP

#include <cassert>
#include <map>
#include "object_listener.hpp"
#include "type_information.hpp"

namespace jsonxx {

template <class T>
class deserializer : public object_listener {
protected:
    typedef T target_struct_type;
    T& t;

    field_info<T> expected_field;

    typedef typename type_information<T>::info_map_t expect_map_t;
    static expect_map_t& expect_from_key;

public:
    deserializer(T&);
    ~deserializer();

    void key(const std::string&);

    void start_object();
    void end_object();

    void start_array();
    void end_array();

    void value(const std::string&);
    void value(int);
    void value(double);
    void value(bool_type);
    void value(null_type);
};

template <class T>
typename deserializer<T>::expect_map_t& deserializer<T>::expect_from_key =
    type_information<T>::info_map;

template <class T>
deserializer<T>::deserializer(T& t_) :
    t(t_)
{
}

template <class T>
deserializer<T>::~deserializer() {
}

template <class T>
void deserializer<T>::key(const std::string& k) {
    typename expect_map_t::const_iterator i = expect_from_key.find(k);
    if (i != expect_from_key.end()) expected_field = i->second;
    else expected_field.type = field_type::t_ignore;
}

template <class T>
void deserializer<T>::start_object() { }
template <class T>
void deserializer<T>::end_object() { }

template <class T>
void deserializer<T>::start_array() { }
template <class T>
void deserializer<T>::end_array() { }

template <class T>
void deserializer<T>::value(const std::string& v) {
    if (expected_field.type == field_type::t_string) t.*expected_field.field.t_string = v;
}

template <class T>
void deserializer<T>::value(int v) {
    if (expected_field.type == field_type::t_int) t.*expected_field.field.t_int = v;
}

template <class T>
void deserializer<T>::value(double v) {
    if (expected_field.type == field_type::t_double) t.*expected_field.field.t_double = v;
}

template <class T>
void deserializer<T>::value(bool_type v) {
    if (expected_field.type == field_type::t_bool) t.*expected_field.field.t_bool = v.value;
}

template <class T>
void deserializer<T>::value(null_type) {
    assert(false);
}

} // namespace jsonxx

#endif // JSONXX_DESERIALIZER_HPP
