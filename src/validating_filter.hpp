#ifndef JSONXX_VALIDATING_FILTER_HPP
#define JSONXX_VALIDATING_FILTER_HPP

#include <stack>
#include <stdexcept>
#include "object_filter.hpp"

#include "declspec.hpp"

namespace jsonxx {

class validation_error : public std::runtime_error {
public:
    validation_error(const char* err) : std::runtime_error(err) { }
};

/*
   This filter validates that the member functions are called in an order
   that represents a valid JSON object. If a violation is found, an exception
   of type valitation_error is thrown. The filter is also put into an error
   state, where no more errors are reported and no events are propagated to
   the target listener.
*/
class JSONXX_DECLSPEC validating_filter : public object_filter {
    enum state_t {
        initial,
        expecting_key,
        expecting_value,
        in_array,
        finished,
        error
    };

    std::stack<state_t> state;

    void validate(bool);

public:
    validating_filter();
    validating_filter(object_listener* target);
    ~validating_filter();

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

#endif // JSONXX_VALIDATING_FILTER_HPP

