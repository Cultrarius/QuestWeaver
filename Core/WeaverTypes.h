//
// Created by michael on 16.10.15.
//

#pragma once

#include <stdint.h>
#include <vector>
#include <string>

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
    };
}
