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
#include "WeaverConfig.h"

namespace weave {

    class QuestWeaver {
    public:
        explicit QuestWeaver(uint64_t seed);

        explicit QuestWeaver(WeaverConfig config);

        std::vector<std::shared_ptr<Quest>> GetQuestsWithState(QuestState state) const;

        std::vector<std::shared_ptr<Quest>> GetAllQuests() const;

        std::shared_ptr<Quest> CreateNewQuest();

        std::shared_ptr<Quest> GetQuest(ID questId) const;

        void Tick(float delta);

        void RegisterTemplateFactory(std::shared_ptr<TemplateFactory> factory);

        std::shared_ptr<Quest> ChangeQuestState(QuestModelAction questAction);

        void ChangeWorkingDirectories(Directories directories);

        const WorldModel &GetWorldModel() const;

        /*
         * Serializes the whole quest system including the quest model and world model. Please note that upon
         * deserializing, the template factories and the world model listeners must be registered again as they are not
         * serialized.
         * The data is serialized in a portable way, so no extra steps must be taken to serialize and deserialize
         * on different platforms.
         */
        void Serialize(std::ostream &outputStream, StreamType type);

        /*
         * Deserializes the quest system including the quest model and world model. Please note that upon
         * deserializing, the template factories and the world model listeners must be registered again as they are not
         * serialized. In addition, the current working directory info must be updated or the default is used.
         *
         * The StreamType parameter must be the same as the one used to serialize the data.
         */
        static QuestWeaver Deserialize(std::istream &inputStream, StreamType type);

        /*
         * Deserializes the quest system including the quest model and world model. Please note that upon
         * deserializing, the template factories and the world model listeners must be registered again as they are not
         * serialized.
         *
         * The StreamType parameter must be the same as the one used to serialize the data.
         */
        static QuestWeaver Deserialize(std::istream &inputStream, StreamType type, Directories currentDirectories);

    private:
        std::unique_ptr<WeaverEngine> engine;
        std::unique_ptr<QuestModel> quests;
        std::unique_ptr<TemplateEngine> templates;
        std::unique_ptr<WorldModel> world;
        std::unique_ptr<StoryWriter> stories;
        std::shared_ptr<RandomStream> randomStream;

        // serialization
        friend class cereal::access;

        QuestWeaver();

        template<class Archive>
        void load(Archive &archive) {
            archive(CEREAL_NVP(randomStream), CEREAL_NVP(quests), CEREAL_NVP(world));
            engine.reset(new WeaverEngine(randomStream));
            stories.reset(new StoryWriter(randomStream, *quests, *templates));
            templates.reset(new TemplateEngine(randomStream, Directories()));
        }

        template<class Archive>
        void save(Archive &archive) const {
            archive(CEREAL_NVP(randomStream), CEREAL_NVP(quests), CEREAL_NVP(world));
        }
    };
}
