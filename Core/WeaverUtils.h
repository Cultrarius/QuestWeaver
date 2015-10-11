//
// Created by michael on 11.10.15.
//

#pragma once

#include <string>
#include <random>

namespace weave {

    class RandomStream {
    private:
        std::uniform_int_distribution<int> defaultDistribution;
        std::default_random_engine generator;
    public:
        explicit RandomStream(uint64_t seed) {
            generator.seed(seed);
        }

        virtual int RandomInt() {
            return defaultDistribution(generator);
        }

        virtual int RandomIntRange(int start, int end) {
            std::uniform_int_distribution<int> limitedDist(start, end);
            return limitedDist(generator);
        }

        virtual void Seed(uint64_t seed) {
            generator.seed(seed);
        }
    };

    bool replace(std::string *str, const std::string &from, const std::string &to);

    void replaceAll(std::string *str, const std::string &from, const std::string &to);


}  // namespace weave
