//
// Created by michael on 14.02.16.
//

#pragma once

#include <Core/WeaverTypes.h>

namespace weave {

    /*!
     * @ingroup storyApi
     */
    class Nugget {
    public:
        Nugget() = default;

        Nugget(std::string key, std::vector<std::string> requiredTypes, std::vector<std::string> texts,
               std::unordered_map<std::string, int> minValues, std::unordered_map<std::string, int> maxValues,
               std::unordered_map<std::string, NameType> randomNames);

        std::string GetKey() const noexcept;

        std::vector<std::string> GetRequiredTypes() const noexcept;

        std::vector<std::string> GetTexts() const noexcept;

        std::set<std::string> GetRandomizationKeys() const noexcept;

        std::string GetRandomValue(std::string key, std::shared_ptr<RandomStream> stream,
                                   const NameGenerator &nameGen) const noexcept;

    private:
        std::string key;
        std::vector<std::string> requiredTypes;
        std::vector<std::string> texts;
        std::set<std::string> randomKeys;
        std::unordered_map<std::string, int> minValues;
        std::unordered_map<std::string, int> maxValues;
        std::unordered_map<std::string, NameType> randomNames;
    };
}