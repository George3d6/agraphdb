#pragma once

#include <string>

namespace data {
class Int64 {
public:
    //members
    const int64_t content;

    //mandatory methods
    std::string serialize() const noexcept {
        return std::to_string(content);
    }

    //Lifetime management
    template <class NumberType>
    Int64(NumberType number) noexcept : content(number) {}
    //Mandatory:
    Int64(std::string serialization) : content(std::stoi(serialization)) {}
};
}
