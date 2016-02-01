//
// Created by michael on 10.08.15.
//


#include <Template/TemplateEngine.h>

using namespace std;
using namespace weave;

shared_ptr<Template> TemplateEngine::GetTemplateForNewQuest() {
    if (factories.size() == 0) {
        throw ContractFailedException("No factory defined to create template.\n");
    }
    auto factoryIndex = randomStream->GetRandomIndex(factories.size());
    shared_ptr<QuestTemplateFactory> factory = factories.at(factoryIndex);
    auto factoryKeys = factory->GetTemplateKeys();
    if (factoryKeys.size() == 0) {
        throw ContractFailedException("No templates defined in template factory.\n");
    }
    auto templateIndex = randomStream->GetRandomIndex(factoryKeys.size());
    auto key = factoryKeys.at(templateIndex);
    cout << "Creating template with key: " << key << endl;
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
    factories.push_back(factory);
}

TemplateEngine::TemplateEngine(std::shared_ptr<RandomStream> randomStream, Directories dirs) :
        randomStream(randomStream), dirs(dirs) {
}

void weave::TemplateEngine::ChangeDirectories(Directories newDirs)
{
	dirs = newDirs;
	for (auto factory : factories) {
		factory->dirs = dirs;
	}
}
