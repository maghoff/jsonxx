#ifndef JSONXX_INDENTING_WRITER_HPP
#define JSONXX_INDENTING_WRITER_HPP

#include "object_listener.hpp"

#include "declspec.hpp"

namespace jsonxx {

class JSONXX_DECLSPEC indenting_writer : public object_listener {
    std::ostream& out;

    enum state_t {
        skip_key_comma,
        skip_array_comma,
        other
    };
    state_t state;

    int indentation_level;

    void newline();
    void comma_unless(state_t);
    void maybe_key_comma();
    void maybe_array_comma();

public:
    // This constructor has the following *side effect*:
    // The boolalpha-flag is *set* on the given ostream. This flag should be
    // set throughout the JSON generation, to ensure proper formatting
    // of boolean values.
    indenting_writer(std::ostream&);
    ~indenting_writer();

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

#endif // INDENTING_WRITER_HPP
