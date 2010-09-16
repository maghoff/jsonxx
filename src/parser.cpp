#include <cassert>
#include <sstream>
#include <stdexcept>
#include "object_listener.hpp"
#include "parser.hpp"

extern "C" {
#include "../import/libjson/json.h"
}

namespace jsonxx {

namespace {

template <typename T>
T my_lexical_cast(const char* buf, size_t length) {
    T res;

    std::stringstream ss;
    ss.write(buf, length);

    ss >> res;

    return res;
}

int parser_callback(void *userdata, int type, const char *data, uint32_t length) {
    object_listener** target_ptr = reinterpret_cast<object_listener**>(userdata);
    object_listener* target = *target_ptr;

    switch (type) {
    case JSON_OBJECT_BEGIN: target->start_object(); break;
    case JSON_OBJECT_END: target->end_object(); break;
    case JSON_ARRAY_BEGIN: target->start_array(); break;
    case JSON_KEY: target->key(std::string(data, length)); break;
    case JSON_STRING: target->value(std::string(data, length)); break;
    case JSON_INT: target->value(my_lexical_cast<int>(data, length)); break;
    case JSON_FLOAT: target->value(my_lexical_cast<float>(data, length)); break;
    case JSON_NULL: target->value(null); break;
    case JSON_TRUE: target->value(bool_type(true)); break;
    case JSON_FALSE: target->value(bool_type(false)); break;
    default:
        assert(false);
        return -1;
    }

    return 0;
}

}

parser::parser(object_listener* target_) :
    p(new json_parser)
{
    target = target_;
    int init_result = json_parser_init(p.get(), NULL, &parser_callback, &target);
    if (init_result != 0) {
        throw std::runtime_error("jsonxx: json_parser_init(...) failed");
    }
}

parser::~parser() {
    json_parser_free(p.get());
}

void parser::parse(const char* begin, const char* end) {
    int ret = json_parser_string(p.get(), begin, end-begin, NULL);
    if (ret != 0) {
        throw std::runtime_error("jsonxx: json_parser_string failed");
    }
}

void parser::parse(const std::string& str) {
    parse(str.data(), str.data() + str.length());
}

void parser::parse(std::istream& in) {
    const size_t bs = 65536;
    char buffer[bs];
    while (in.good()) {
        in.read(buffer, bs);
        parse(buffer, buffer + in.gcount());
    }
}

bool parser::is_done() const {
    return json_parser_is_done(p.get());
}

} // namespace jsonxx
