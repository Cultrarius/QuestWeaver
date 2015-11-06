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

        std::vector<std::shared_ptr<Quest>> GetQuestsWithState(QuestState state) const;

        std::shared_ptr<Quest> CreateNewQuest();

        void Tick(float delta);

    private:
        std::unique_ptr<WeaverEngine> engine;
        std::unique_ptr<QuestModel> quests;
        std::unique_ptr<TemplateEngine> templates;
        std::unique_ptr<WorldModel> world;
        std::shared_ptr<RandomStream> randomStream;

        void updateWorld(const std::vector<ModelAction> &modelActions, const Quest &quest,
                         const std::vector<QuestPropertyValue> &questProperties);
    };
}
