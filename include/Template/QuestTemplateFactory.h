//
// Created by michael on 15.08.15.
//

#pragma once

#include "../Core/WeaverTypes.h"
#include "../Core/WeaverUtils.h"
#include "../json/json.h"
#include "QuestTemplate.h"

namespace weave {
    class QuestTemplateFactory {
    public:
        virtual ~QuestTemplateFactory() = default;

        std::vector<std::string> GetTemplateKeys();

        std::shared_ptr<QuestTemplate> CreateTemplate(const std::string &templateKey);

    protected:
        virtual std::shared_ptr<QuestTemplate> createFromJsonValues(const Json::Value &root) const = 0;

        virtual const char *getTemplateFile() const = 0;

        std::unordered_map<std::string, Json::Value> templateMap;

        std::vector<TemplateQuestProperty> extractProperties(const Json::Value &root) const;

        std::vector<TemplateQuestDescription> extractDescriptions(const Json::Value &root) const;

        std::string extractTitle(const Json::Value &root) const;

        void extractProperties(std::vector<TemplateQuestProperty> *properties,
                               const Json::Value &jsonMandatory,
                               bool isMandatory) const;

        std::shared_ptr<RandomStream> randomStream;

    private:
        friend class TemplateEngine;

        bool isInitialized = false;

        Directories dirs;

        void initialize();
    };
}
