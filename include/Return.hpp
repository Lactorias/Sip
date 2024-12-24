#ifndef RETURN
#define RETURN

#include <SipVariant.hpp>
#include <strstream>


struct Return : public std::runtime_error {
    Object& value;

    Return(Object& value) : value(value), std::runtime_error("hey returne") {}
};










#endif // RETURN