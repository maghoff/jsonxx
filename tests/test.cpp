#include "test_util.hpp"

extern bool compact_writer_tests();
extern bool indenting_writer_tests();
extern bool raii_tests();
extern bool validating_filter_tests();
extern bool util_tests();

int main() {
    bool ok = true;

    ok &= compact_writer_tests();
    ok &= indenting_writer_tests();
    ok &= raii_tests();
    ok &= validating_filter_tests();
    ok &= util_tests();

    return ok ? 0 : 1;
}
