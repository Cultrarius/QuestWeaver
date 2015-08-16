//
// Created by michael on 15.08.15.
//

#pragma once

#include <memory>
#include <unordered_map>
#include "../json/json.h"
#include "Template.h"

class TemplateFactory {
public:
    virtual std::vector<std::string> GetTemplateKeys();

    virtual std::shared_ptr<Template> CreateTemplate(std::string templateKey) = 0;
protected:
    Json::Value readTemplateFile(const char *fileName);

    std::unordered_map<std::string, Json::Value> templateMap;
};


