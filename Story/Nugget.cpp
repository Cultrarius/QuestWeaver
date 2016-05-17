//
// Created by michael on 14.02.16.
//

#include "Story/Nugget.h"

using namespace std;
using namespace weave;

Nugget::Nugget(string key, float rarity, vector<string> requiredTypes, vector<string> texts,
               unordered_map<string, int> minValues, unordered_map<string, int> maxValues,
               unordered_map<string, NameType> randomNames) :
        key(key), rarity(rarity), requiredTypes(requiredTypes), texts(texts), minValues(minValues),
        maxValues(maxValues), randomNames(randomNames) {
    for (auto minVal : randomNames) {
        randomKeys.insert(minVal.first);
    }
    for (auto minVal : minValues) {
        randomKeys.insert(minVal.first);
    }
    for (auto maxVal : maxValues) {
        randomKeys.insert(maxVal.first);
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

set<string> Nugget::GetRandomizationKeys() const noexcept {
    return randomKeys;
}

string Nugget::GetRandomValue(string key, shared_ptr<RandomStream> stream,
                              const NameGenerator &nameGen) const noexcept {
    if (randomKeys.count(key) == 0) {
        return "";
    }

    auto nameIter = randomNames.find(key);
    if (nameIter != randomNames.end()) {
        return nameGen.CreateName(nameIter->second, stream);
    }

    int minValue = 0;
    auto minIter = minValues.find(key);
    bool hasMin = minIter != minValues.end();
    if (hasMin) {
        minValue = minIter->second;
    }

    int maxValue = 100;
    auto maxIter = maxValues.find(key);
    bool hasMax = maxIter != maxValues.end();
    if (hasMax) {
        maxValue = maxIter->second;
    }
    if (minValue > maxValue) {
        if (hasMin && !hasMax) {
            maxValue = minValue;
        } else {
            minValue = maxValue;
        }
    }
    return to_string(stream->GetIntInRange(minValue, maxValue));
}

float Nugget::GetRarity() const noexcept {
    return rarity;
}
