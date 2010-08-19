#ifndef JSONXX_TEST_TEST_UTIL_HPP
#define JSONXX_TEST_TEST_UTIL_HPP

#include <iostream>

// The error messages are formatted like GCC's error messages, to allow an IDE
// to pick them up as error messages.
#define REPORT(msg) \
    std::cerr << __FILE__ << ':' << __LINE__ << ": error: " msg << std::endl;

#define CHECK_EQUAL(a, b) \
    if ((a) != (b)) { \
        REPORT( \
            "Failed test: " #a " == " #b " " \
            "(" << (a) << " != " << (b) << ')' \
        ) \
        ok = false; \
    }

bool execute(bool(*f)(), const char* f_name);
#define EXEC(f) execute(f, #f)

#endif // JSONXX_TEST_TEST_UTIL_HPP
