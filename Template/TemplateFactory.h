//
// Created by michael on 15.08.15.
//

#pragma once

#include "../json/json.h"

class TemplateFactory {
public:
    TemplateFactory();

    virtual std::vector<std::string> GetTemplateKeys() = 0;

protected:
    Json::Value readTemplateFile(const char *fileName);
};


