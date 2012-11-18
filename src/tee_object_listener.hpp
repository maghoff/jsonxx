#ifndef JSONXX_TEE_OBJECT_LISTENER_HPP
#define JSONXX_TEE_OBJECT_LISTENER_HPP

#include "object_listener.hpp"

#include "declspec.hpp"

namespace jsonxx {

class JSONXX_DECLSPEC tee_object_listener : public object_listener {
    object_listener& a;
    object_listener& b;

public:
    tee_object_listener(object_listener&, object_listener&);
    ~tee_object_listener();

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

#undef JSONXX_DECLSPEC

#endif // JSONXX_TEE_OBJECT_LISTENER_HPP
