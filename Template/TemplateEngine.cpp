//
// Created by michael on 10.08.15.
//


#include "TemplateEngine.h"
#include <iostream>

using namespace std;
using namespace weave;

shared_ptr<Template> TemplateEngine::GetTemplateForNewQuest() {
    // TODO choose factory
    shared_ptr<TemplateFactory> factory = factories[0];
    auto factoryKeys = factory->GetTemplateKeys();
    for (auto key : factoryKeys) {
        cout << "Creating template with key: " << key << endl;
        return factory->CreateTemplate(key);
    }
    throw new runtime_error("No factory defined to create template.\n");
}

void TemplateEngine::RegisterTemplateFactory(std::shared_ptr<TemplateFactory> factory) {
    for (auto f : factories) {
        if (f.get() == factory.get()) {
            return;
        }
    }
    factories.push_back(factory);
}
