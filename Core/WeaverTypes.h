//
// Created by michael on 16.10.15.
//

#pragma once

#include <stdint.h>
#include <string>

namespace weave {
    typedef uint64_t ID;

    struct ContractFailedException : public std::exception {
        std::string s;

        explicit ContractFailedException(std::string ss) : s(ss) { }

        virtual ~ContractFailedException() throw() { }

        const char *what() const throw() { return s.c_str(); }
    };
}
