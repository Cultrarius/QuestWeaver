//
// Created by michael on 10.08.15.
//


#include "TemplateEngine.h"
#include <iostream>

using namespace std;

std::shared_ptr<Template> TemplateEngine::GetTemplateForNewQuest() {
    shared_ptr<TemplateFactory> factory = factories[0]; //TODO choose factory
    auto factoryKeys = factory->GetTemplateKeys();
    std::shared_ptr<Template> result;
    for (auto key : factoryKeys) {
        cout << "Creating template with key: " << key << endl;
        result = factory->CreateTemplate(key);
    }

    return result;
}

void TemplateEngine::RegisterTemplateFactory(std::shared_ptr<TemplateFactory> factory) {
    for (auto f : factories) {
        if (f.get() == factory.get()) {
            return;
        }
    }
    factories.push_back(factory);
}
