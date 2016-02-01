//
// Created by michael on 10.08.15.
//

#pragma once

#include <Core/WeaverTypes.h>
#include <cereal.h>
#include <json/json.h>
#include <Core/WeaverEngine.h>
#include <QuestModel/QuestModel.h>
#include <Template/TemplateEngine.h>
#include <World/WorldModel.h>
#include <QuestModel/Quest.h>
#include <Core/WeaverUtils.h>
#include <World/WorldListener.h>
#include <WeaverConfig.h>

namespace weave {

    /*
     * This class represents a complete quest system and is the main entry point for an application to create and
     * manage quests. To be usable, the caller has to provide a world model and matching template factories from which
     * the quest weaver will generate new and interesting quest.
     *
     * Instances of this class are not thread safe.
     */
    class QuestWeaver {
    public:
        explicit QuestWeaver(uint64_t seed);

        /*
         * Creates a new weaver instance with the given config.
         * The config needs to contain a valid world model pointer.
         * The quest weaver instance will take ownership of the world model and delete it when being destroyed.
         * @param config the config to use - must contain a valid world model
         */
        explicit QuestWeaver(WeaverConfig config);

        /*
         * Registers a new template factory with the quest system.
         * Usually, the factories are already registered when creating an instance using the WeaverConfig parameter.
         * However, this is not true when creating a new instance by deserialiation.
         * This method *MUST* be used on newly deserialized objects, otherwise it is impossible to create new quests
         */
        void RegisterTemplateFactory(std::shared_ptr<TemplateFactory> factory);

        /*
         * Changes the directories used by the quest system to load files such as templates.
         */
        void ChangeWorkingDirectories(Directories directories);

        /*
         * Creates a new quest.
         * This node does not have any input because it is the responsibility of the quest weaver to decide which quest
         * should be next.
         * The previously registered template factories and the world model are used to create the quest.
         *
         * Once a quest is created, it is active in the world (and most likely already changed it).
         * The caller of this API cannot decide to undo that or delete the quest, it can only change the state to
         * a "completed" type.
         */
        std::shared_ptr<Quest> CreateNewQuest();

        /*
         * Advances the quest system state by ticking all quests and executing their desired world and quest changes.
         * Until this method is called, quests are unable to change the world model or their state.
         *
         * *WARNING* After ticking, all objects received via this API might be invalid!
         * They can still be used, but might contain outdated data.
         */
        void Tick(float delta);

        /*
         * Returns a list of all quests with the given state.
         */
        std::vector<std::shared_ptr<Quest>> GetQuestsWithState(QuestState state) const;

        /*
         * Returns a list of all quests
         */
        std::vector<std::shared_ptr<Quest>> GetAllQuests() const;

        /*
         * Returns the quest with the given ID.
         */
        std::shared_ptr<Quest> GetQuest(ID questId) const;

        /*
         * Directly changes the state of a quest.
         * A quest can usually change its state once it is ticked,
         * this method should only be used if that is not possible.
         */
        std::shared_ptr<Quest> ChangeQuestState(QuestModelAction questAction);

        /*
         * Returns a immutable reference to the world model - to change the world model, use the Tick() method which
         * gathers changes from the quests.
         *
         * After the quest system was deserialized, the world model listener has to be registered again.
         */
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
            stories.reset(new StoryWriter(randomStream, *quests, *templates, Directories()));
            templates.reset(new TemplateEngine(randomStream, Directories()));
        }

        template<class Archive>
        void save(Archive &archive) const {
            archive(CEREAL_NVP(randomStream), CEREAL_NVP(quests), CEREAL_NVP(world));
        }
    };
}
