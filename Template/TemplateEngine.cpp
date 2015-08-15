//
// Created by michael on 10.08.15.
//


#include "TemplateEngine.h"

Template TemplateEngine::GetTemplateForNewQuest() {
    return Template();
}

void TemplateEngine::RefreshTemplates() {

}

void TemplateEngine::RegisterTemplateFactory(std::shared_ptr<TemplateFactory> factory) {
    for (auto f : factories) {
        if (f.get() == factory.get()) {
            return;
        }
    }
    factories.push_back(factory);
}
