#ifndef JSONXX_COMPACT_WRITER_HPP
#define JSONXX_COMPACT_WRITER_HPP

#include "writer_base.hpp"

#include "declspec.hpp"

namespace jsonxx {

class JSONXX_DECLSPEC compact_writer : public writer_base {
    enum state_t {
        skip_key_comma,
        skip_array_comma,
        other
    };
    state_t state;

    void comma_unless(state_t);
    void prepare_for_key();
    void prepare_for_value();

public:
    compact_writer(std::ostream&);
    ~compact_writer();

    void key(const std::string&);

    void start_object();
    void end_object();
    void start_array();
};

} // namespace jsonxx

#undef JSONXX_DECLSPEC

#endif // JSONXX_COMPACT_WRITER_HPP

