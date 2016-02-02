//
// Created by michael on 11.10.15.
//

#pragma once

#include <json/json.h>
#include "WeaverTypes.h"


namespace weave {

    bool replace(std::string *str, const std::string &from, const std::string &to);

    void replaceAll(std::string *str, const std::string &from, const std::string &to);

    std::string htmlEncloseWithTag(const std::string &str, const std::string &tag,
                                   const std::vector<std::string> &classes);

    std::string htmlEncloseWithTag(const std::string &str, const std::string &tag);

    std::string htmlEncloseWithTag(const std::string &str, const std::string &tag, const std::string &cssClass);

    Json::Value readJsonFromFile(const char *fileName, const Directories &dirs);
}
