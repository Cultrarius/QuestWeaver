//
// Created by michael on 15.08.15.
//

#pragma once

#include <memory>
#include <unordered_map>
#include "../json/json.h"
#include "Template.h"

namespace weave {
    class TemplateFactory {
    public:
        virtual std::vector<std::string> GetTemplateKeys();

        std::shared_ptr<Template> CreateTemplate(const std::string &templateKey) const;

    protected:
        virtual std::shared_ptr<Template> CreateTemplate(const Json::Value &root) const = 0;

        Json::Value readTemplateFile(const char *fileName);

        std::unordered_map<std::string, Json::Value> templateMap;

        std::vector<TemplateQuestProperty> extractProperties(const Json::Value &root) const;

        std::vector<TemplateQuestDescription> extractDescriptions(const Json::Value &root) const;

        std::vector<std::string> extractTitles(const Json::Value &root) const;

        void extractProperties(std::vector<TemplateQuestProperty> *properties,
                               const Json::Value &jsonMandatory,
                               bool isMandatory) const;
    };
}

