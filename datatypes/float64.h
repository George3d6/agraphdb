#pragma once

#include <string>

namespace data {
class Float64 {
public:
    //members
    double content;

    //mandatory methods
    std::string serialize() const noexcept {
        return std::to_string(content);
    }

    //Lifetime management
    template <class NumberType>
    Float64(NumberType number) noexcept : content(number) {}
    Float64(const Float64 & data):content(data.content) {}

    template <class NumberType>
    Float64& operator=(NumberType & new_content) {
        this->content = new_content;
        return *this;
    }
    //Mandatory:
    Float64(std::string serialization) noexcept : content(std::stod(serialization)) {}
};
}
