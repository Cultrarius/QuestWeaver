//
// Created by michael on 15.08.15.
//

#pragma once

#include <memory>
#include <unordered_map>
#include <fstream>
#include "../json/json.h"
#include "Template.h"

namespace weave {
    class TemplateFactory {
    public:
        explicit TemplateFactory(std::shared_ptr<RandomStream> randomStream) : randomStream(randomStream) { }

        virtual std::vector<std::string> GetTemplateKeys();

        std::shared_ptr<Template> CreateTemplate(const std::string &templateKey) const;

        void SetTemplateDirectory(std::string templateDirectory);

        void SetModDirectory(std::string modDirectory);

    protected:
        virtual std::shared_ptr<Template> createFromJsonValues(const Json::Value &root) const = 0;

        Json::Value readTemplateFile(const char *fileName);

        std::unordered_map<std::string, Json::Value> templateMap;

        std::vector<TemplateQuestProperty> extractProperties(const Json::Value &root) const;

        std::vector<TemplateQuestDescription> extractDescriptions(const Json::Value &root) const;

        std::string extractTitle(const Json::Value &root) const;

        void extractProperties(std::vector<TemplateQuestProperty> *properties,
                               const Json::Value &jsonMandatory,
                               bool isMandatory) const;

    private:
        std::shared_ptr<RandomStream> randomStream;

        void openFile(const char *name, std::ifstream *stream);

        std::string templateDirectory = "../Data/Template/";

        std::string modDirectory = "../Mods/Template/";
    };
}

