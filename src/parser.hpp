#ifndef JSONXX_PARSER_HPP
#define JSONXX_PARSER_HPP

#include <istream>
#include <memory>
#include <string>

#include "declspec.hpp"

#ifdef _MSC_VER
#pragma warning(push)
// C4251: Warning about std::auto_ptr<json_parser> not being dll-exported.
#pragma warning(disable: 4251)
#endif

struct json_parser;

namespace jsonxx {

class object_listener;

class JSONXX_DECLSPEC parser {
    std::auto_ptr<json_parser> p;

    // This class is not copyable
    parser(const parser&);
    parser& operator = (const parser&);

public:
    parser(object_listener* target);
    ~parser();

    object_listener* target;

    void parse(const char* begin, const char* end);
    void parse(const std::string&);

    // Will consume the entire istream
    void parse(std::istream&);

    bool is_done() const;
};

} // namespace jsonxx

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#undef JSONXX_DECLSPEC

#endif // JSONXX_PARSER_HPP
