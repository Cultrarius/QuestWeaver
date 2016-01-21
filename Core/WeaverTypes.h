//
// Created by michael on 16.10.15.
//

#pragma once

#include "../cereal.h"

namespace weave {
    typedef uint64_t ID;

    struct ContractFailedException : public std::exception {
        std::string s;

        explicit ContractFailedException(std::string ss) : s(ss) { }

        virtual ~ContractFailedException() throw() { }

        const char *what() const throw() { return s.c_str(); }
    };

    struct EnumClassHash {
        template<typename T>
        std::size_t operator()(T t) const {
            return static_cast<std::size_t>(t);
        }
    };

    struct Directories {
        std::string templateDirectory = "../Template/";

        std::string modDirectory = "../Mods/Template/";
    private:
        // serialization
        friend class cereal::access;

        template<class Archive>
        void serialize(Archive &archive) {
            archive(CEREAL_NVP(templateDirectory), CEREAL_NVP(modDirectory));
        }
    };
}
