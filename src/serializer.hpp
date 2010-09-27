#ifndef JSONXX_SERIALIZER_HPP
#define JSONXX_SERIALIZER_HPP

#include <cassert>
#include "object_listener.hpp"
#include "type_information.hpp"

namespace jsonxx {

template <class T>
void serializer(jsonxx::object_listener& o, const T& t) {
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

} // namespace jsonxx

#endif // JSONXX_SERIALIZER_HPP
