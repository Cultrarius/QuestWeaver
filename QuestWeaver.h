//
// Created by michael on 10.08.15.
//

#pragma once

#include "Core/WeaverTypes.h"
#include "cereal.h"
#include "json/json.h"
#include "Core/WeaverEngine.h"
#include "QuestModel/QuestModel.h"
#include "Template/TemplateEngine.h"
#include "World/WorldModel.h"
#include "QuestModel/Quest.h"
#include "Core/WeaverUtils.h"
#include "World/WorldListener.h"

namespace weave {
    class QuestWeaver {
    public:
        explicit QuestWeaver(uint64_t seed);

        QuestWeaver(uint64_t seed, Directories directories);

        std::vector<std::shared_ptr<Quest>> GetQuestsWithState(QuestState state) const;

        std::vector<std::shared_ptr<Quest>> GetAllQuests() const;

        std::shared_ptr<Quest> CreateNewQuest();

        std::shared_ptr<Quest> GetQuest(ID questId) const;

        void Tick(float delta);

        void RegisterWorldListener(std::shared_ptr<WorldListener> listener);

        std::shared_ptr<Quest> ChangeQuestState(QuestModelAction questAction);

    private:
        std::unique_ptr<WeaverEngine> engine;
        std::unique_ptr<QuestModel> quests;
        std::unique_ptr<TemplateEngine> templates;
        std::unique_ptr<WorldModel> world;
        std::unique_ptr<StoryWriter> stories;
        std::shared_ptr<RandomStream> randomStream;
    };
}
