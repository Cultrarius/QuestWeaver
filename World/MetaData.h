//
// Created by michael on 26.10.15.
//

#pragma once

#include "cereal.h"
#include "../Core/WeaverTypes.h"
#include "../Core/WeaverUtils.h"

class MetaData {
public:
    MetaData();

    explicit MetaData(const std::string &name, const std::string &stringValue);

    explicit MetaData(const std::string &name, int value);

    MetaData(const std::string &name, const std::string &stringValue, int value);

    int GetValue() const;

    std::string ToString() const;

    std::string GetName() const;

private:
    std::string name;

    std::string stringValue;

    int value;

    // serialization
    friend class cereal::access;

    template<class Archive>
    void serialize(Archive &archive) {
        archive(CEREAL_NVP(name), CEREAL_NVP(stringValue), CEREAL_NVP(value));
    }
};

