//
// Created by michael on 16.10.15.
//

#pragma once

#include <random>
#include "../cereal.h"

namespace weave {

    /*
     * Used to identify unique objects in the quest system, i.e. quests or world entities.
     */
    typedef uint64_t ID;

    /*
     * The main exception type used by all classes of the QuestWeaver system.
     */
    struct ContractFailedException : public std::exception {
        std::string s;

        explicit ContractFailedException(std::string ss) : s(ss) { }

        virtual ~ContractFailedException() throw() { }

        const char *what() const throw() { return s.c_str(); }
    };

    /*
     * Operator to enable the use of enums as keys in maps
     */
    struct EnumClassHash {
        template<typename T>
        std::size_t operator()(T t) const {
            return static_cast<std::size_t>(t);
        }
    };

    struct Directories {
        /*
         * The base directory which all template factories use to search for template files.
         */
        std::string templateDirectory = "../Resources/";

        /*
         * Template factories prefer template files found in this directory, so mods can override the templates
         * in the default directory.
         */
        std::string modDirectory = "../Mods/";
    private:
        // serialization
        friend class cereal::access;

        template<class Archive>
        void serialize(Archive &archive) {
            archive(CEREAL_NVP(templateDirectory), CEREAL_NVP(modDirectory));
        }
    };

    enum class StreamType {
        /*
         * A verbose text format, good for debugging or manual changes.
         * Big size and slow compared to the binary format.
         */
                JSON,

        /*
         * A compact binary format without compression. Fastest option.
         */
                BINARY
    };

    /*
     * Defines a serializable, seeded pseudo-random number generator that is used to generate random numbers for the
     * quest system.
     */
    class RandomStream {
    public:
        RandomStream() {
            generator.seed(std::default_random_engine::default_seed);
        }

        explicit RandomStream(uint64_t seed) {
            generator.seed(seed);
        }

        virtual ~RandomStream() = default;

        /*
         * Returns a random int (without any range limit)
         */
        virtual int GetInt() {
            return defaultDistribution(generator);
        }

        /*
         * Returns a random int from a given range.
         */
        virtual int GetIntInRange(int start, int end) {
            if (end < start) {
                throw ContractFailedException("(End < Start) for random distribution in range.\n");
            }
            std::uniform_int_distribution<int> limitedDist(start, end);
            return limitedDist(generator);
        }

        /*
         * Returns a random unsigned long from a given range.
         */
        virtual uint64_t GetULongInRange(uint64_t start, uint64_t end) {
            if (end < start) {
                throw ContractFailedException("(End < Start) for random distribution in range.\n");
            }
            std::uniform_int_distribution<uint64_t> limitedDist(start, end);
            return limitedDist(generator);
        }

        /*
         * Reseeds the random stream.
         */
        virtual void Seed(uint64_t seed) {
            generator.seed(seed);
        }

        /*
         * Returns a random array index for an array of the given size.
         */
        virtual uint64_t GetRandomIndex(uint64_t collectionSize) {
            if (collectionSize == 0) {
                throw ContractFailedException("Collection Size = 0 for random index.\n");
            }
            return GetULongInRange(0, collectionSize - 1);
        }

    private:
        std::uniform_int_distribution<int> defaultDistribution;
        std::mt19937 generator;

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
}
