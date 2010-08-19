#ifndef JSONXX_INDENTING_WRITER_HPP
#define JSONXX_INDENTING_WRITER_HPP

#include "writer_base.hpp"

#include "declspec.hpp"

namespace jsonxx {

class JSONXX_DECLSPEC indenting_writer : public writer_base {
    enum state_t {
        start_of_object,
        in_key_value_pair,
        start_of_array,
        other
    };
    state_t state;

    int indentation_level;

    void newline();
    void prepare_for_key();
    void prepare_for_value();

public:
    indenting_writer(std::ostream&);
    ~indenting_writer();

    void key(const std::string&);

    void start_object();
    void end_object();

    void start_array();
    void end_array();
};

} // namespace jsonxx

#undef JSONXX_DECLSPEC

#endif // INDENTING_WRITER_HPP
