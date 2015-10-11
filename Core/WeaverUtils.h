//
// Created by michael on 11.10.15.
//

#pragma once

#include <string>

namespace weave {

    bool replace(std::string *str, const std::string &from, const std::string &to) {
        size_t start_pos = str->find(from);
        if (start_pos == std::string::npos) {
            return false;
        }
        str->replace(start_pos, from.length(), to);
        return true;
    }

    void replaceAll(std::string *str, const std::string &from, const std::string &to) {
        if (from.empty()) {
            return;
        }
        size_t start_pos = 0;
        while ((start_pos = str->find(from, start_pos)) != std::string::npos) {
            str->replace(start_pos, from.length(), to);
            start_pos += to.length();
        }
    }


}  // namespace weave
