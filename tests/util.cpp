#include <sstream>
#include "test_util.hpp"
#include "util.hpp"

namespace {

bool simple_quoted_string() {
    bool ok = true;

    std::stringstream ss;
    jsonxx::write_quoted_string(ss, "simple");
    CHECK_EQUAL(ss.str(), "\"simple\"");

    return ok;
}

bool quoted_string_with_control_chars() {
    bool ok = true;

    std::stringstream ss;
    jsonxx::write_quoted_string(ss,
        "Multiline\n\ttext\r\nwith bells\b and whistles\f\n"
        "Quotes: \"\n"
        "Backslash: \\\n"
    );
    CHECK_EQUAL(
        ss.str(),
        "\""
        "Multiline\\n\\ttext\\r\\nwith bells\\b and whistles\\f\\n"
        "Quotes: \\\"\\n"
        "Backslash: \\\\\\n"
        "\""
    );

    return ok;
}

bool invalid_control_char() {
    try {
        std::stringstream ss;
        jsonxx::write_quoted_string(ss, "\x01");
    }
    catch(const jsonxx::encoding_error&) {
        return true;
    }

    REPORT("Control should have been diverted by an exception");
    return false;
}

}

bool util_tests() {
    bool ok = true;

    ok &= EXEC(simple_quoted_string);
    ok &= EXEC(quoted_string_with_control_chars);
    ok &= EXEC(invalid_control_char);

    return ok;
}
