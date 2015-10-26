//
// Created by michael on 26.10.15.
//

#include "MetaData.h"

MetaData::MetaData() : MetaData("", "", 0) {
}

MetaData::MetaData(const std::string &name, const std::string &stringValue) : MetaData(name, stringValue, 0) {
}

MetaData::MetaData(const std::string &name, int value) : MetaData(name, "", value) {
}

MetaData::MetaData(const std::string &name, const std::string &stringValue, int value) :
        name(name), stringValue(stringValue), value(value) {
}

int MetaData::GetValue() const {
    return value;
}

std::string MetaData::ToString() const {
    return stringValue;
}

std::string MetaData::GetName() const {
    return name;
}
