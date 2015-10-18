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

namespace weave {
    class QuestWeaver {
    public:
        explicit QuestWeaver(uint64_t seed);

        std::list<std::shared_ptr<Quest>> GetActiveQuests() const;

        std::shared_ptr<Quest> CreateNewQuest();

    private:
        std::unique_ptr<WeaverEngine> engine;
        std::unique_ptr<QuestModel> quests;
        std::unique_ptr<TemplateEngine> templates;
        std::unique_ptr<WorldModel> world;
        std::shared_ptr<RandomStream> randomStream;

        void updateWorld(const Quest& quest);
    };
}
