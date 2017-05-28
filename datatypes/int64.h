#pragma once

#include <string>

namespace data {
class Int64 {
public:
    //members
    int64_t content;

    //mandatory methods
    std::string serialize() const noexcept {
        return std::to_string(content);
    }

    template <class NumberType>
    Int64(NumberType number) noexcept : content(number) {}
    Int64(const Int64 & data):content(data.content) {}

    template <class NumberType>
    Int64& operator=(NumberType & new_content) {
        this->content = new_content;
        return *this;
    }

    //Mandatory:
    Int64(std::string serialization) : content(std::stoi(serialization)) {}
};
}
