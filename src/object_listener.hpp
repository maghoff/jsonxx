#ifndef JSON_SRC_OBJECT_LISTENER_HPP
#define JSON_SRC_OBJECT_LISTENER_HPP

#include <string>

namespace jsonxx {

// Use this class to accept bools more precisely in an overload-situation.
// Any pointer -- for example const char* -- is implicitly convertible to
// bool by a built-in conversion, which takes precedence to user-defined
// conversions, such as std::string(const char*).
struct bool_helper {
    bool value;

    bool_helper(bool value_) : value(value_) { }
};

class null { };

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
    virtual void value(bool_helper) = 0;
    virtual void value(null) = 0;
};

}

#endif
