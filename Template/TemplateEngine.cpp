//
// Created by michael on 10.08.15.
//


#include "TemplateEngine.h"
#include <iostream>

using namespace std;
using namespace weave;

shared_ptr<Template> TemplateEngine::GetTemplateForNewQuest(std::shared_ptr<RandomStream> randomStream) {
    if (factories.size() == 0) {
        throw "No factory defined to create template.\n";
    }
    int factoryIndex = randomStream->GetRandomIndex(factories.size());
    shared_ptr<TemplateFactory> factory = factories[factoryIndex];
    auto factoryKeys = factory->GetTemplateKeys();
    if (factoryKeys.size() == 0) {
        throw "No templates defined in template factory.\n";
    }
    int templateIndex = randomStream->GetRandomIndex(factoryKeys.size());
    auto key = factoryKeys[templateIndex];
    cout << "Creating template with key: " << key << endl;
    return factory->CreateTemplate(key);
}

void TemplateEngine::RegisterTemplateFactory(std::shared_ptr<TemplateFactory> factory) {
    for (auto f : factories) {
        if (f.get() == factory.get()) {
            return;
        }
    }
    factories.push_back(factory);
}
