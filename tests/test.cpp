#include "test_util.hpp"

extern bool compact_writer_tests();
extern bool indenting_writer_tests();
extern bool parser_tests();
extern bool raii_tests();
extern bool serializing_tests();
extern bool util_tests();
extern bool validating_filter_tests();

int main() {
    bool ok = true;

    ok &= compact_writer_tests();
    ok &= indenting_writer_tests();
    ok &= parser_tests();
    ok &= raii_tests();
    ok &= serializing_tests();
    ok &= util_tests();
    ok &= validating_filter_tests();

    return ok ? 0 : 1;
}
