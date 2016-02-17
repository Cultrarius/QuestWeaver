//
// Created by michael on 11.10.15.
//

#pragma once

#include <json/json.h>
#include "WeaverTypes.h"


namespace weave {

    bool replace(std::string *str, const std::string &from, const std::string &to) noexcept;

    void replaceAll(std::string *str, const std::string &from, const std::string &to) noexcept;

    std::string htmlEncloseWithTag(const std::string &str, const std::string &tag,
                                   const std::vector<std::string> &classes) noexcept;

    std::string htmlEncloseWithTag(const std::string &str, const std::string &tag) noexcept;

    std::string htmlEncloseWithTag(const std::string &str, const std::string &tag,
                                   const std::string &cssClass) noexcept;

    Json::Value readJsonFromFile(const char *fileName, const Directories &dirs);
}
