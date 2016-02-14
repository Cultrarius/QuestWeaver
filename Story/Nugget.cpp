//
// Created by michael on 14.02.16.
//

#include "Story/Nugget.h"

using namespace std;
using namespace weave;

Nugget::Nugget(std::string key, std::vector<std::string> requiredTypes, std::vector<std::string> texts) :
        key(key), requiredTypes(requiredTypes), texts(texts) {
}

std::string Nugget::GetKey() {
    return key;
}

std::vector<std::string> Nugget::GetRequiredTypes() {
    return requiredTypes;
}

std::vector<std::string> Nugget::GetTexts() {
    return texts;
}
