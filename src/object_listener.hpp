#ifndef JSON_SRC_OBJECT_LISTENER_HPP
#define JSON_SRC_OBJECT_LISTENER_HPP

#include <string>

namespace jsonxx {

// This type is used to avoid problems with string literals (const char*)
// implicitly being cast to bool. It is assumed that the user is less likely
// to use bool values than string literal values.
struct bool_type {
    bool value;
    explicit bool_type(bool value_) : value(value_) { }
};

class null_type { };
extern null_type null;

class object_listener {
public:
    virtual ~object_listener();

    virtual void key(const std::string&) = 0;

    virtual void start_object() = 0;
    virtual void end_object() = 0;

    virtual void start_array() = 0;
    virtual void end_array() = 0;

    virtual void value(const std::string&) = 0;
    virtual void value(int) = 0;
    virtual void value(double) = 0;
    virtual void value(bool_type) = 0;
    virtual void value(null_type) = 0;

    // If you don't want to use the overloaded interface:
    void value_bool(bool b) { value(bool_type(b)); }
    void value_null() { value(null); }
};

}

#endif