#include <cassert>
#include "validating_filter.hpp"

namespace jsonxx {

validating_filter::validating_filter()
{
}

validating_filter::validating_filter(object_listener* target_) :
    object_filter(target_)
{
    state.push(initial);
}

validating_filter::~validating_filter()
{
}

void validating_filter::validate(bool ok) {
    if (ok) return;
    state.push(error);
    throw validation_error("jsonxx validation error");
}

void validating_filter::key(const std::string& key) {
    if (state.top() == error) return;
    validate(state.top() == expecting_key);
    state.top() = expecting_value;
    object_filter::key(key);
}

void validating_filter::start_object() {
    if (state.top() == error) return;
    validate(state.top() == initial || state.top() == expecting_value || state.top() == in_array);
    if (state.top() == initial) state.top() = finished;
    if (state.top() == expecting_value) state.top() = expecting_key;
    state.push(expecting_key);
    object_filter::start_object();
}

void validating_filter::end_object() {
    if (state.top() == error) return;
    validate(state.top() == expecting_key);
    state.pop();
    assert(state.empty() == false);
    assert(state.top() == in_array || state.top() == expecting_key || state.top() == finished);
    object_filter::end_object();
}

void validating_filter::start_array() {
    if (state.top() == error) return;
    validate(state.top() == expecting_value || state.top() == in_array);
    if (state.top() == expecting_value) state.top() = expecting_key;
    state.push(in_array);
    object_filter::start_array();
}

void validating_filter::end_array() {
    if (state.top() == error) return;
    validate(state.top() == in_array);
    state.pop();
    assert(state.top() == in_array || state.top() == expecting_key || state.top() == finished);
    object_filter::end_array();
}

void validating_filter::value(const std::string& v) {
    if (state.top() == error) return;
    validate(state.top() == expecting_value || state.top() == in_array);
    if (state.top() == expecting_value) state.top() = expecting_key;
    object_filter::value(v);
}

void validating_filter::value(int v) {
    if (state.top() == error) return;
    validate(state.top() == expecting_value || state.top() == in_array);
    if (state.top() == expecting_value) state.top() = expecting_key;
    object_filter::value(v);
}

void validating_filter::value(double v) {
    if (state.top() == error) return;
    validate(state.top() == expecting_value || state.top() == in_array);
    if (state.top() == expecting_value) state.top() = expecting_key;
    object_filter::value(v);
}

void validating_filter::value(bool_type v) {
    if (state.top() == error) return;
    validate(state.top() == expecting_value || state.top() == in_array);
    if (state.top() == expecting_value) state.top() = expecting_key;
    object_filter::value(v);
}

void validating_filter::value(null_type v) {
    if (state.top() == error) return;
    validate(state.top() == expecting_value || state.top() == in_array);
    if (state.top() == expecting_value) state.top() = expecting_key;
    object_filter::value(v);
}

} // namespace jsonxx
