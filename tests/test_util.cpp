#include "test_util.hpp"

bool execute(bool(*f)(), const char* f_name) {
    bool result = f();
    if (!result) {
        std::cerr << "Test failed: " << f_name << std::endl;
    }
    return result;
}

