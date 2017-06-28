#pragma once

#include <memory>

#include "libs/rapidjson/document.h"
#include "libs/rapidjson/writer.h"
#include "libs/rapidjson/stringbuffer.h"

namespace data {
class Json {
public:
    //members
    rapidjson::Document content;

    //mandatory methods
    std::string serialize() const noexcept {
        rapidjson::StringBuffer buffer;
        rapidjson::Writer writer(buffer);
        content.Accept(writer);
        return buffer.GetString();
    }

    //Lifetime management
    template <class StringType>
    Json(StringType json) {
        content.Parse(json);
    }
    Json(const Json & json):content(json.content) {}

    template <class StringType>
    Json& operator=(StringType & new_content) {
        this->content.Parse(new_content);
        return *this;
    }
    //Mandatory:
    Json(std::string serialization) noexcept : {
        content.Parse(json);
    }

    //GetValue for various types
    template<template <class...> class ValueType>
    std::unique_ptr<ValueType> get_value(std::string member_path) {
        if(!content.HasMember(member_path)) {
            return nullptr;
        }

        if(typeid(ValueType) == typeid(int)) {
           return std::make_unique<std::string>{content[member_path].GetInt()};
        }
        if(typeid(ValueType) == typeid(double) || typeid(ValueType) == typeid(float)) {
           return std::make_unique<std::string>{content[member_path].GetDouble()};
        }
        if(typeid(ValueType) == typeid(std::string)) {
           return std::make_unique<std::string>{content[member_path].GetString()};
        }
        if(typeid(ValueType) == typeid(std::string)) {
           return std::make_unique<std::string>{content[member_path].GetString()};
        }

    }
};
}
