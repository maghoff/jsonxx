#ifndef JSONXX_WRITER_BASE_HPP
#define JSONXX_WRITER_BASE_HPP

#include <ostream>
#include "object_listener.hpp"

#include "declspec.hpp"

namespace jsonxx {

class JSONXX_DECLSPEC writer_base : public object_listener {
protected:
    std::ostream& out;

    virtual void prepare_for_key() = 0;
    virtual void prepare_for_value() = 0;

public:
    writer_base(std::ostream&);
    ~writer_base();

    void key(const std::string&);

    void start_object();
    void end_object();

    void start_array();
    void end_array();

    void value(const std::string&);
    void value(int);
    void value(double);
    void value(bool_type);
    void value(null_type);
};

} // namespace jsonxx

#undef JSONXX_DECLSPEC

#endif // JSONXX_WRITER_BASE_HPP
