#ifndef JSONXX_OBJECT_FILTER_HPP
#define JSONXX_OBJECT_FILTER_HPP

#include "object_listener.hpp"

#include "declspec.hpp"

namespace jsonxx {

// The lifetime of the target-object must be controlled outside of this class.
class JSONXX_DECLSPEC object_filter : public object_listener {
public:
    object_filter();
    object_filter(object_listener* target);

    object_listener* target;

    // The (default) implementations of the object_listener-interface just
    // forwards to the target-object. The pointer will not be checked for 0.

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

#endif // JSONXX_OBJECT_FILTER_HPP

