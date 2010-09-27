#ifndef JSONXX_OBJECT_LISTENER_HPP
#define JSONXX_OBJECT_LISTENER_HPP

#include <string>

#include "declspec.hpp"

namespace jsonxx {

/*doc*
### `bool_type` ###

This type is used to avoid problems with string literals (`const char*`)
implicitly being cast to bool. It is assumed that the user is less likely
to use bool values than string literal values. This is the entire definition:

    :::c++
    struct bool_type {
        bool value;
        explicit bool_type(bool value_) : value(value_) { }
    };
*/
struct bool_type {
    bool value;
    explicit bool_type(bool value_) : value(value_) { }
};

/*doc*
### `null_type` ###

`null_type` is a class representing the special JSON value "null". When
needing an instance of this class, it is possible to use the global variable
`jsonxx::null`. This is the entire definition:

    :::c++
    class null_type { };
*/
class null_type { };
extern JSONXX_DECLSPEC null_type null;


/*doc*
### `object_listener` ###

`object_listener` is the streaming JSON-API used throughout the JSONxx library.

The equivalent to this interface in the world of XML is the SAX API.

*/
class JSONXX_DECLSPEC object_listener {
    // This class is not copyable
    object_listener(const object_listener&);
    object_listener& operator = (const object_listener&);

public:
    object_listener() { }
    virtual ~object_listener();

    /*doc*
          :::c++
          void key(const std::string&)

      A call to this function represents the key in a key-value-pair in an
      object in a JSON document. */
    virtual void key(const std::string&) = 0;

    /*doc*
          :::c++
          void start_object()

      A call to this function represents the start of an object (a dictionary)
      in a JSON document. This is represented by a `{` in JSON. */
    virtual void start_object() = 0;

    /*doc*
          :::c++
          void end_object()

      A call to this function represents the end of an object (a dictionary)
      in a JSON document. This is represented by a `}` in JSON. */
    virtual void end_object() = 0;

    /*doc*
          :::c++
          void start_array()

      A call to this function represents the start of an array in a JSON
      document. This is represented by a `[` in JSON. */
    virtual void start_array() = 0;

    /*doc*
          :::c++
          void end_array()

      A call to this function represents the end of an array in a JSON
      document. This is represented by a `]` in JSON. */
    virtual void end_array() = 0;

    /*doc*
          :::c++
          void value(const std::string&); // For string values
          void value(int); // For number values that have no fractional part
          void value(double); // For other number values
          void value(bool_type); // For boolean values
          void value(null_type); // For the special value "null"

      A call to a function of the name value represents a value in a
      key-value-pair in an object or a value in an array. Each overload
      represents a JSON type as noted above.
    */
    virtual void value(const std::string&) = 0;
    virtual void value(int) = 0;
    virtual void value(double) = 0;
    virtual void value(bool_type) = 0;
    virtual void value(null_type) = 0;

    /*doc*
          :::c++
          void value_bool(bool b)

      This is equivalent to `value(bool_type(b))`, and has been included for
      convenience. (See also [`bool_type`](#bool_type)) */
    void value_bool(bool b) { value(bool_type(b)); }

    /*doc*
          :::c++
          void value_null()

      This is equivalent to `value(null)`. (See [`null_type`](#null_type)) */
    void value_null() { value(null); }

    // Convenience:
    void value(void(*f)(object_listener&)) { f(*this); }
    void value(void(*f)(object_listener*)) { f(this); }

    /*doc*
          :::c++
          void pair(string key, T value)

      This is equivalent to:

          :::c++
          this->key(key);
          this->value(value);

      This function has been included for your convenience. */
    template <typename T>
    void pair(const std::string& k, T v) {
        key(k);
        value(v);
    }
};

class scoped_object {
    object_listener& obj;
public:
    scoped_object(object_listener& obj_) : obj(obj_) { obj.start_object(); }
    ~scoped_object() { obj.end_object(); }
};

class scoped_array {
    object_listener& obj;
public:
    scoped_array(object_listener& obj_) : obj(obj_) { obj.start_array(); }
    ~scoped_array() { obj.end_array(); }
};

} // namespace jsonxx

#undef JSONXX_DECLSPEC

#endif // JSONXX_OBJECT_LISTENER_HPP

