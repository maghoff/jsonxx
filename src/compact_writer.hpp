#ifndef JSON_SRC_COMPACT_WRITER_HPP
#define JSON_SRC_COMPACT_WRITER_HPP

#include <ostream>
#include "object_listener.hpp"

namespace json {

    class compact_writer : public object_listener {
        std::ostream& out;
        bool skip_comma;

        void maybe_comma();

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
        void value(bool_helper);
        void value(null);
    };

    void write_quoted_string(std::ostream&, const std::string&);
}

#endif

