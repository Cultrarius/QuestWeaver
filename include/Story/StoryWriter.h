//
// Created by michael on 21.12.15.
//

#pragma once

#include <Core/WeaverUtils.h>
#include <Core/Graph/WeaverGraph.h>
#include <QuestModel/QuestModel.h>
#include <Template/TemplateEngine.h>

namespace weave {
    class StoryWriter {
    public:
        explicit StoryWriter(std::shared_ptr<RandomStream> randomStream, const weave::QuestModel &model,
                             const weave::TemplateEngine &templateEngine);

        std::string CreateStory(const weave::WeaverGraph &graph) const;

    private:
        std::shared_ptr<RandomStream> rs;
        const weave::QuestModel &model;
        const weave::TemplateEngine &templateEngine;
        mutable bool isInitialized = false;
        Directories dirs;

        void initialize() const;
    };
}
