//
// Created by michael on 14.02.16.
//

#include "Story/Nugget.h"

using namespace std;
using namespace weave;

Nugget::Nugget(string key, vector<string> requiredTypes, vector<string> texts, unordered_map<string, int> minValues,
               unordered_map<string, int> maxValues) :
        key(key), requiredTypes(requiredTypes), texts(texts), minValues(minValues), maxValues(maxValues) {
    for (auto minVal : minValues) {
        randomKeys.push_back(minVal.first);
    }
    for (auto maxVal : maxValues) {
        if (minValues.count(maxVal.first) == 0) {
            randomKeys.push_back(maxVal.first);
        }
    }
}

string Nugget::GetKey() const noexcept {
    return key;
}

vector<string> Nugget::GetRequiredTypes() const noexcept {
    return requiredTypes;
}

vector<string> Nugget::GetTexts() const noexcept {
    return texts;
}

vector<string> Nugget::GetRandimizationKeys() const noexcept {
    return randomKeys;
}

int Nugget::GetRandomValue(string key, RandomStream stream) const noexcept {
    int minValue = 0;
    auto minIter = minValues.find(key);
    if (minIter != minValues.end()) {
        minValue = minIter->second;
    }
    int maxValue = 100;
    auto maxIter = maxValues.find(key);
    if (maxIter != maxValues.end()) {
        maxValue = maxIter->second;
    }
    if (minValue > maxValue) {
        minValue = maxValue;
    }
    return stream.GetIntInRange(minValue, maxValue);
}




