#ifndef JSONXX_TYPE_INFORMATION_HPP
#define JSONXX_TYPE_INFORMATION_HPP

#include <map>
#include <string>

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

}

#endif // JSONXX_TYPE_INFORMATION_HPP
