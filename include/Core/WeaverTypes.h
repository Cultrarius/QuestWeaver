//
// Created by michael on 16.10.15.
//

#pragma once

#include <random>
#include "../cereal.h"

namespace weave {

    /*!
     * Used to identify unique objects in the quest system, i.e. quests or world entities.
     *
     * @ingroup mainApi
     */
    typedef uint64_t ID;

    /*!
     * The main exception type used by all classes of the QuestWeaver system.
     *
     * All wrong API calls or expected problems (e.g. not being able to parse a quest template) will throw this
     * exception.
     *
     * @ingroup mainApi
     */
    struct ContractFailedException : public std::exception {
        std::string s;

        explicit ContractFailedException(std::string ss) : s(ss) {}

        virtual ~ContractFailedException() throw() {}

        const char *what() const throw() { return s.c_str(); }
    };

    /*!
     * Operator to enable the use of enums as keys in maps.
     *
     * @ingroup internal
     */
    struct EnumClassHash {
        template<typename T>
        std::size_t operator()(T t) const {
            return static_cast<std::size_t>(t);
        }
    };

    /*!
     * This struct hold information about the programs file search paths.
     *
     * @ingroup mainApi
     */
    struct Directories {
        /*!
         * The base directory which all template factories use to search for template files.
         */
        std::string templateDirectory = "Resources/";

        /*!
         * Template factories prefer template files found in this directory, so mods can override the templates
         * in the default directory.
         */
        std::string modDirectory = "../Resources/";
    private:
        // serialization
        friend class cereal::access;

        template<class Archive>
        void serialize(Archive &archive) {
            archive(CEREAL_NVP(templateDirectory), CEREAL_NVP(modDirectory));
        }
    };

    /*!
     * Defines the possible serialization types of the quest system.
     *
     * @ingroup mainApi
     */
    enum class StreamType {
        /*!
         * A verbose text format, good for debugging or manual changes.
         * Big size and slow compared to the binary format.
         */
                JSON,

        /*!
         * A compact binary format without compression; fastest option.
         */
                BINARY
    };

    /*!
     * Defines the possible output formats of the texts produced by the quest system.
     *
     * @ingroup mainApi
     */
    enum class FormatterType {
        /*!
         * All texts will be created as vanilla texts without any additional formatting markers
         */
                TEXT,

        /*!
         * A number of html tags will be injected into the created text to allow the output to be formatted with
         * css classes.
         */
                HTML
    };

    /*!
     * The logger used to print errors and debug messages.
     *
     * The logger can also be configured to eat non-breaking exceptions (e.g. release mode) and prevent them
     * from being thrown.
     * Be aware that this might cause weird behavior!
     *
     * @ingroup mainApi
     */
    class Logger {
    private:
        static std::shared_ptr<Logger> sharedInstance;

    protected:
        virtual void error(const std::string &what) {
            std::cerr << "ERROR: " << what << std::endl;
        }

        virtual void debug(const std::string &what) {
            std::cout << what << "\n";
        }

    public:
        bool EatsNonFatalExceptions = false;

        static void Error(const std::string &what) {
            if (sharedInstance) {
                sharedInstance->error(what);
            }
        }

        static void Error(ContractFailedException ex) {
            if (sharedInstance) {
                sharedInstance->error(ex.what());
                if (!sharedInstance->EatsNonFatalExceptions) {
                    throw ex;
                }
            } else {
                throw ex;
            }
        }

        static void Fatal(const std::string &what) {
            Fatal(ContractFailedException(what));
        }

        static void Fatal(ContractFailedException ex) {
            if (sharedInstance) {
                sharedInstance->error(ex.what());
            }
            throw ex;
        }

        static void Debug(const std::string &what, uint8_t nesting = 0) {
            if (sharedInstance) {
                std::string space;
                for (uint8_t i = 0; i < nesting; i++) {
                    space.append("  ");
                }
                sharedInstance->debug(space + what);
            }
        }

        static std::shared_ptr<Logger> Get() {
            return sharedInstance;
        }

        static void Set(std::shared_ptr<Logger> newInstance) {
            sharedInstance = newInstance;
        }
    };

    /*!
     * RNG used by the quest system.
     *
     * Defines a serializable, seeded pseudo-random number generator that is used to generate random numbers for the
     * quest system.
     *
     * @ingroup mainApi
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
         * Returns a random int (without any range limit).
         */
        virtual int GetInt() {
            return defaultDistribution(generator);
        }

        /*
         * Returns a random int from a given range.
         */
        virtual int GetIntInRange(int start, int end) {
            if (end < start) {
                Logger::Error(ContractFailedException("(End < Start) for random distribution in range.\n"));
                end = start;
            }
            std::uniform_int_distribution<int> limitedDist(start, end);
            return limitedDist(generator);
        }

        /*
         * Returns a random unsigned long from a given range.
         */
        virtual uint64_t GetULongInRange(uint64_t start, uint64_t end) {
            if (end < start) {
                Logger::Error(ContractFailedException("(End < Start) for random distribution in range.\n"));
                end = start;
            }
            std::uniform_int_distribution<uint64_t> limitedDist(start, end);
            return limitedDist(generator);
        }

        /*
         * Returns a normal distributed random integer from a given range.
         */
        virtual int GetNormalIntInRange(int start, int end) {
            if (end < start) {
                Logger::Error(ContractFailedException("(End < Start) for normal distribution in range.\n"));
                end = start;
            }
            int mean = (end + start) / 2.0f;
            float stddev = abs(end - mean) / 2.5f;
            std::normal_distribution<float> normalDist(mean, stddev);
            int result = round(normalDist(generator));
            return result < start ? start : (result > end ? end : result);
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
                Logger::Fatal("Collection Size = 0 for random index.\n");
                return 0;
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

    enum class NameType {
        DARK_PERSON,
        DARK_THING,
        LIGHT_PERSON,
        LIGHT_THING,
        FUNNY,
        ALIEN,
        CORPORATION,
        SHIP,
        CUDDLY,
        CUDDLY_VERB,
        VENERABLE
    };

    class NameGenerator {
    public:
        virtual std::string CreateName(NameType nameType, std::shared_ptr<RandomStream> random) const {
            return "DefaultName" + std::to_string(static_cast<int>(nameType)) + std::to_string(random->GetInt());
        }
    };
}
