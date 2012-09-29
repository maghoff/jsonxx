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

bool write_code_point_as_utf8_ascii() {
    bool ok = true;

    std::stringstream ss;
    jsonxx::write_code_point_as_utf8(ss, 'a');
    CHECK_EQUAL("a", ss.str());

    return ok;
}

bool write_code_point_as_utf8_2_octets() {
    bool ok = true;

    std::stringstream ss;
    jsonxx::write_code_point_as_utf8(ss, 0x00E5);
    CHECK_EQUAL("\xC3\xA5", ss.str());

    return ok;
}

bool write_code_point_as_utf8_3_octets() {
    bool ok = true;

    std::stringstream ss;
    jsonxx::write_code_point_as_utf8(ss, 0x20AC);
    CHECK_EQUAL("\xE2\x82\xAC", ss.str());

    return ok;
}

bool write_code_point_as_utf8_4_octets() {
    bool ok = true;

    std::stringstream ss;
    jsonxx::write_code_point_as_utf8(ss, 0x01F4A9);
    CHECK_EQUAL("\xF0\x9F\x92\xA9", ss.str());

    return ok;
}

bool write_code_point_as_utf8_fail_on_out_of_range() {
    try {
        std::stringstream ss;
        jsonxx::write_code_point_as_utf8(ss, 0xFFFFFFFF);
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
    ok &= EXEC(write_code_point_as_utf8_ascii);
    ok &= EXEC(write_code_point_as_utf8_2_octets);
    ok &= EXEC(write_code_point_as_utf8_3_octets);
    ok &= EXEC(write_code_point_as_utf8_4_octets);
    ok &= EXEC(write_code_point_as_utf8_fail_on_out_of_range);

    return ok;
}
