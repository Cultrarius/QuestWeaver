//
// Created by michael on 10.08.15.
//

#include <algorithm>
#include <QuestWeaver.h>
#include <Template/Space/SpaceQuestTemplateFactory.h>
#include <World/Space/SpaceWorldModel.h>
#include <Story/Space/CommonSpaceStoryFactory.h>

using namespace std;
using namespace weave;

QuestWeaver::QuestWeaver(WeaverConfig config) {
    if (config.worldModel == nullptr) {
        throw ContractFailedException("A world model must be provided for the quest system to work.");
    }

    if (config.randomStream != nullptr) {
        randomStream.reset(config.randomStream);
    } else {
        randomStream.reset(new RandomStream(config.seed));
    }
    engine.reset(new WeaverEngine(randomStream));
    quests.reset(new QuestModel());
    templates.reset(new TemplateEngine(randomStream, config.dirs, config.formatterType));

    config.worldModel->rs = randomStream;
    world.reset(config.worldModel);

    stories.reset(new StoryWriter(randomStream, *quests, *templates, *world, config.dirs));
    for (auto factory : config.questTemplateFactories) {
        RegisterQuestTemplateFactory(factory);
    }
    for (auto factory : config.storyTemplateFactories) {
        RegisterStoryTemplateFactory(factory);
    }
}

shared_ptr<Quest> QuestWeaver::CreateNewQuest() {
    auto questTemplate = templates->GetTemplateForNewQuest();
    EngineResult result = engine->fillTemplate(questTemplate, *quests, *world, *stories);
    world->Execute(result.GetModelActions());
    shared_ptr<Quest> newQuest = questTemplate->ToQuest(result.GetQuestPropertyValues(), result.GetStory());
    return quests->RegisterNew(newQuest, result.GetQuestPropertyValues());
}

std::vector<std::shared_ptr<Quest>> QuestWeaver::GetQuestsWithState(QuestState state) const {
    return quests->GetQuestsWithState(state);
}

void QuestWeaver::Tick(float delta) {
    for (const auto &quest : quests->GetQuests()) {
        const QuestTickResult &change = quest->Tick(delta);
        world->Execute(change.GetWorldChanges());
        quests->Execute(change.GetQuestChange());
    }
}

std::vector<std::shared_ptr<Quest>> QuestWeaver::GetAllQuests() const {
    return quests->GetQuests();
}

shared_ptr<Quest> QuestWeaver::GetQuest(ID questId) const {
    return quests->GetQuest(questId);
}

std::shared_ptr<Quest> QuestWeaver::ChangeQuestState(QuestModelAction questAction) {
    return quests->Execute(questAction);
}

void QuestWeaver::RegisterQuestTemplateFactory(std::shared_ptr<QuestTemplateFactory> factory) {
    templates->RegisterTemplateFactory(factory);
}

void QuestWeaver::RegisterStoryTemplateFactory(std::shared_ptr<StoryTemplateFactory> factory) {
    stories->RegisterTemplateFactory(factory);
}

void QuestWeaver::Serialize(std::ostream &outputStream, StreamType type) {
    if (type == StreamType::JSON) {
        cereal::JSONOutputArchive outputArchive(outputStream);
        outputArchive(*this);
    } else if (type == StreamType::BINARY) {
        cereal::PortableBinaryOutputArchive outputArchive(outputStream);
        outputArchive(*this);
    } else {
        throw ContractFailedException("Unknown serialization type!");
    }
}

QuestWeaver QuestWeaver::Deserialize(std::istream &inputStream, StreamType type) {
    QuestWeaver deserialized;
    if (type == StreamType::JSON) {
        cereal::JSONInputArchive inputArchive(inputStream);
        inputArchive(deserialized);
    } else if (type == StreamType::BINARY) {
        cereal::PortableBinaryInputArchive inputArchive(inputStream);
        inputArchive(deserialized);
    } else {
        throw ContractFailedException("Unknown serialization type!");
    }
    return deserialized;
}

QuestWeaver QuestWeaver::Deserialize(std::istream &inputStream, StreamType type, Directories currentDirectories) {
    QuestWeaver deserialized = Deserialize(inputStream, type);
    deserialized.ChangeWorkingDirectories(currentDirectories);
    return deserialized;
}

QuestWeaver::QuestWeaver() {
}

void weave::QuestWeaver::ChangeWorkingDirectories(Directories directories) {
    templates->ChangeDirectories(directories);
    stories->ChangeDirectories(directories);
}

const WorldModel &QuestWeaver::GetWorldModel() const {
    return *world;
}
