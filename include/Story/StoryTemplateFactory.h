//
// Created by michael on 21.12.15.
//

#pragma once

#include <Core/WeaverUtils.h>
#include <Story/StoryTemplate.h>

namespace weave {
    /*!
     * @ingroup storyApi
     */
    class StoryTemplateFactory {
    public:
        virtual ~StoryTemplateFactory() = default;

        std::map<std::string, std::shared_ptr<StoryTemplate>> GetTemplates();

        virtual std::string GetNuggetFolder() const = 0;

    protected:
        virtual std::shared_ptr<StoryTemplate> createFromJsonValues(const Json::Value &root) const = 0;

        virtual const char *getTemplateFile() const = 0;

        std::set<std::string> readRequired(const Json::Value &templateJson) const;

        std::vector<RawStoryLine> readRawLines(const Json::Value &templateJson) const;

    private:
        friend class StoryWriter;

        bool isInitialized = false;

        Directories dirs;

        std::map<std::string, std::shared_ptr<StoryTemplate>> templates;

        void initialize();
    };
}
