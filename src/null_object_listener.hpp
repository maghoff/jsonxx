#ifndef JSONXX_NULL_OBJECT_LISTENER_HPP
#define JSONXX_NULL_OBJECT_LISTENER_HPP

#include "object_listener.hpp"

namespace jsonxx {

// This object_listener does nothing, and is useful for testing filters.
class null_object_listener : public object_listener {
public:
    null_object_listener();

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

} // namespace jsonxx

#endif // NULL_OBJECT_LISTENER_HPP
