#pragma once
#include <string>

namespace data {
class UTFString {
public:
    //members
    std::string content;

    //mandatory methods
    std::string serialize() const noexcept {
        return content;
    }

    //Lifetime management
    template <class StringType>
    UTFString(StringType string) noexcept : content(string) {}
    UTFString(const UTFString & data):content(data.content) {}

    template <class StringType>
    UTFString& operator=(StringType & new_content) {
        this->content = new_content;
        return *this;
    }
    //Mandatory:
    UTFString(std::string serialization) noexcept : content((serialization)) {}
};
}
