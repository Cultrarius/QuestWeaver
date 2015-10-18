//
// Created by michael on 11.10.15.
//

#pragma once

#include <random>
#include "cereal.h"
#include "WeaverTypes.h"


namespace weave {

    class RandomStream {
    public:
        RandomStream() {
            generator.seed(std::default_random_engine::default_seed);
        }

        explicit RandomStream(uint64_t seed) {
            generator.seed(seed);
        }

        virtual int GetInt() {
            return defaultDistribution(generator);
        }

        virtual int GetIntInRange(int start, int end) {
            if (end < start) {
                throw ContractFailedException("(End < Start) for random distribution in range.\n");
            }
            std::uniform_int_distribution<int> limitedDist(start, end);
            return limitedDist(generator);
        }

        virtual void Seed(uint64_t seed) {
            generator.seed(seed);
        }

        virtual int GetRandomIndex(uint64_t collectionSize) {
            return GetIntInRange(0, static_cast<int>(collectionSize - 1));
        }

    private:
        std::uniform_int_distribution<int> defaultDistribution;
        std::default_random_engine generator;

        // serialization
        friend class cereal::access;

        template<class Archive>
        void save(Archive &archive) const {
            std::stringstream ss;
            ss << generator;
            archive(cereal::make_nvp("seed", ss.str()));
        }

        template<class Archive>
        void load(Archive &archive) {
            std::stringstream ss;
            std::string s;
            archive(cereal::make_nvp("seed", s));
            ss << s;
            ss >> generator;
        }
    };

    bool replace(std::string *str, const std::string &from, const std::string &to);

    void replaceAll(std::string *str, const std::string &from, const std::string &to);

}  // namespace weave
