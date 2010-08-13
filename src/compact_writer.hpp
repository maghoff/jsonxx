#ifndef JSONXX_SRC_COMPACT_WRITER_HPP
#define JSONXX_SRC_COMPACT_WRITER_HPP

#include <ostream>
#include "object_listener.hpp"

namespace jsonxx {

class compact_writer : public object_listener {
    std::ostream& out;

    enum state_t {
        skip_key_comma,
        skip_array_comma,
        other
    };
    state_t state;

    void comma_unless(state_t);
    void maybe_key_comma();
    void maybe_array_comma();

public:
    // This constructor has the following *side effect*:
    // The boolalpha-flag is *set* on the given ostream.
    compact_writer(std::ostream&);
    ~compact_writer();

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

void write_quoted_string(std::ostream&, const std::string&);

}

#endif

