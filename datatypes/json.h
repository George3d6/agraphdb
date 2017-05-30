#pragma once

namespace data {
class Json {
public:
    //members
    std::string content;

    //mandatory methods
    std::string serialize() const noexcept {
        return "!!PANIC!!";
    }

    //Lifetime management
    template <class JsonType>
    Json(JsonType json) noexcept : content(json) {}
    Json(const Json & json):content(json.content) {}

    template <class JsonType>
    Json& operator=(JsonType & new_content) {
        this->content = new_content;
        return *this;
    }
    //Mandatory:
    Json(std::string serialization) noexcept : {

    }
};
}
