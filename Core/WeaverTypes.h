//
// Created by michael on 16.10.15.
//

#pragma once

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
        std::string templateDirectory = "../Template/";

        /*
         * Template factories prefer template files found in this directory, so mods can override the templates
         * in the default directory.
         */
        std::string modDirectory = "../Mods/Template/";
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
}
