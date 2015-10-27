//
// Created by michael on 26.10.15.
//

#include "MetaData.h"

using namespace weave;
using namespace std;

bool MetaData::HasValue(const std::string &name) const {
    return data.find(name) != data.end();
}

int MetaData::GetValue(const std::string &name) const {
    if (HasValue(name)) {
        return data.find(name)->second;
    } else {
        return 0;
    }
}

void MetaData::SetValue(const std::string &name, int value) {
    data[name] = value;
}

vector<string> MetaData::GetValueNames() const {
    vector<string> names;
    for (auto pair : data) {
        names.push_back(pair.first);
    }
    return names;
}
