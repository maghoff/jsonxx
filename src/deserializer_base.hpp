#ifndef JSONXX_DESERIALIZER_BASE_HPP
#define JSONXX_DESERIALIZER_BASE_HPP

#include <cassert>
#include <map>
#include "object_listener.hpp"

namespace jsonxx {

template <class T>
class deserializer_base : public object_listener {
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
    void value(bool_type);
    void value(null_type);
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
    if (expected_field.type == t_string) t.*expected_field.field.t_string = v;
}

template <class T>
void deserializer_base<T>::value(int v) {
    if (expected_field.type == t_int) t.*expected_field.field.t_int = v;
}

template <class T>
void deserializer_base<T>::value(double v) {
    if (expected_field.type == t_double) t.*expected_field.field.t_double = v;
}

template <class T>
void deserializer_base<T>::value(bool_type v) {
    if (expected_field.type == t_bool) t.*expected_field.field.t_bool = v.value;
}

template <class T>
void deserializer_base<T>::value(null_type) {
    assert(false);
}

} // namespace jsonxx

#endif // JSONXX_DESERIALIZER_BASE_HPP