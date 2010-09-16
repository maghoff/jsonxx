#ifndef JSONXX_DESERIALIZER_HPP
#define JSONXX_DESERIALIZER_HPP

#include <cassert>
#include <map>
#include "object_listener.hpp"

namespace jsonxx {

namespace field_type {
    enum field_type {
        t_ignore,
        t_string,
        t_int,
        t_double,
        t_bool
    };
}

template <class T>
union field_pointer {
    std::string T::* t_string;
    int T::* t_int;
    double T::* t_double;
    bool T::* t_bool;
};

template <class T>
struct field_info {
    field_pointer<T> field;
    field_type::field_type type;

    field_info() { }
    field_info(std::string T::* p) : type(field_type::t_string) { field.t_string = p; }
    field_info(int T::* p) : type(field_type::t_int) { field.t_int = p; }
    field_info(double T::* p) : type(field_type::t_double) { field.t_double = p; }
    field_info(bool T::* p) : type(field_type::t_bool) { field.t_bool = p; }
};

template <class T, class K>
field_info<T> make_field_info(K T::* p) { return field_info<T>(p); }

template <class T>
struct type_information {
    typedef std::map<std::string, field_info<T> > info_map_t;
    static info_map_t info_map;
};

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
