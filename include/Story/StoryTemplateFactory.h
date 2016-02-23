//
// Created by michael on 21.12.15.
//

#pragma once

#include <Core/WeaverUtils.h>
#include <Story/StoryTemplate.h>

namespace weave {
    class StoryTemplateFactory {
    public:
        virtual ~StoryTemplateFactory() = default;

        std::vector<std::shared_ptr<StoryTemplate>> GetTemplates();

        virtual std::string GetNuggetFolder() const = 0;

    protected:
        virtual std::shared_ptr<StoryTemplate> createFromJsonValues(const Json::Value &root) const = 0;

        virtual const char *getTemplateFile() const = 0;

        std::vector<std::string> readRequired(const Json::Value &templateJson) const;

        std::vector<RawStoryLine> readRawLines(const Json::Value &templateJson) const;

    private:
        friend class StoryWriter;

        bool isInitialized = false;

        Directories dirs;

        std::vector<std::shared_ptr<StoryTemplate>> templates;

        void initialize();
    };
}
