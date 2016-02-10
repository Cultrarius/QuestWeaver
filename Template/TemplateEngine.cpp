//
// Created by michael on 10.08.15.
//


#include <Template/TemplateEngine.h>

using namespace std;
using namespace weave;

shared_ptr<QuestTemplate> TemplateEngine::GetTemplateForNewQuest() {
    if (factories.size() == 0) {
        throw ContractFailedException("No factory defined to create template.\n");
    }
    auto factoryIndex = randomStream->GetRandomIndex(factories.size());
    shared_ptr<QuestTemplateFactory> factory = factories.at(factoryIndex);
    auto factoryKeys = factory->GetTemplateKeys();
    auto templateIndex = randomStream->GetRandomIndex(factoryKeys.size());
    auto key = factoryKeys.at(templateIndex);
    return factory->CreateTemplate(key);
}

void TemplateEngine::RegisterTemplateFactory(std::shared_ptr<QuestTemplateFactory> factory) {
    for (auto f : factories) {
        if (f.get() == factory.get()) {
            return;
        }
    }
    factory->randomStream = randomStream;
    factory->dirs = dirs;
    factory->formatterType = format;
    factories.push_back(factory);
}

TemplateEngine::TemplateEngine(std::shared_ptr<RandomStream> randomStream, Directories dirs, FormatterType format) :
        randomStream(randomStream), dirs(dirs), format(format) {
}

void weave::TemplateEngine::ChangeDirectories(Directories newDirs)
{
	dirs = newDirs;
	for (auto factory : factories) {
		factory->dirs = dirs;
	}
}

FormatterType TemplateEngine::GetFormat() const {
    return format;
}

