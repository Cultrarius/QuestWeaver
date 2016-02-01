//
// Created by michael on 21.12.15.
//

#pragma once

#include <Core/WeaverUtils.h>
#include <Core/Graph/WeaverGraph.h>
#include <QuestModel/QuestModel.h>
#include <Template/TemplateEngine.h>
#include <Story/StoryTemplateFactory.h>

namespace weave {
    class Nugget {

    };

    class StoryWriter {
    public:
        explicit StoryWriter(std::shared_ptr<RandomStream> randomStream, const weave::QuestModel &model,
                             const weave::TemplateEngine &templateEngine, const Directories &dirs);

        std::string CreateStory(const weave::WeaverGraph &graph) const;

        void ChangeDirectories(const Directories &newDirs);

        void RegisterTemplateFactory(std::shared_ptr<StoryTemplateFactory> factory);

    private:
        std::shared_ptr<RandomStream> rs;
        const weave::QuestModel &model;
        const weave::TemplateEngine &templateEngine;
        Directories dirs;
        std::vector<std::shared_ptr<StoryTemplateFactory>> factories;

        mutable bool isInitialized = false;
        mutable std::vector<Nugget> nuggets;

        void initialize() const;

        void readNuggets() const;
    };
}
