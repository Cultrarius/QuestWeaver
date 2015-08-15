//
// Created by michael on 10.08.15.
//

#pragma once

#include "Template.h"
#include "TemplateFactory.h"
#include <memory>

class TemplateEngine {
public:
    Template GetTemplateForNewQuest();

    void RefreshTemplates();

    void RegisterTemplateFactory(std::shared_ptr<TemplateFactory> factory);

private:
    std::vector<std::shared_ptr<TemplateFactory>> factories;
};