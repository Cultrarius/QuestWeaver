//
// Created by michael on 26.10.15.
//

#include <World/MetaData.h>

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

MetaData &MetaData::SetValue(const std::string &name, int value) {
    data[name] = value;
    return *this;
}

vector<string> MetaData::GetValueNames() const {
    vector<string> names;
    for (auto pair : data) {
        names.push_back(pair.first);
    }
    return names;
}
