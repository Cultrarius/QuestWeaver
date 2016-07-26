//
// Created by michael on 10.08.15.
//


#include <Template/TemplateEngine.h>

using namespace std;
using namespace weave;

vector<shared_ptr<QuestTemplate>> TemplateEngine::GetTemplatesForNewQuest(const WorldModel &worldModel,
                                                                          const QuestModel &questModel) {
    if (factories.size() == 0) {
        Logger::Fatal(ContractFailedException("No factory defined to create template."));
    }
    vector<shared_ptr<QuestTemplate>> templates;
    for (auto &factory : factories) {
        auto factoryKeys = factory->GetTemplateKeys();
        for (auto key : factoryKeys) {
            auto questTemplate = factory->CreateTemplate(key);
            if (questTemplate->IsValid(worldModel, questModel)) {
                templates.push_back(questTemplate);
            }
        }
    }
    return templates;
}

void TemplateEngine::RegisterTemplateFactory(std::unique_ptr<QuestTemplateFactory> factory) {
    factory->randomStream = randomStream;
    factory->dirs = dirs;
    factory->formatterType = format;
    factories.push_back(std::move(factory));
}

TemplateEngine::TemplateEngine(std::shared_ptr<RandomStream> randomStream, Directories dirs, FormatterType format) :
        randomStream(randomStream), dirs(dirs), format(format) {
}

void weave::TemplateEngine::ChangeDirectories(Directories newDirs) {
    dirs = newDirs;
    for (const auto &factory : factories) {
        factory->dirs = dirs;
    }
}

FormatterType TemplateEngine::GetFormat() const {
    return format;
}

