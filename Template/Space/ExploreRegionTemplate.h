//
// Created by michael on 14.08.15.
//

#pragma once

#include "../Template.h"

class ExploreRegionTemplate : public Template {
public:
    Quest ToQuest() const;

    ExploreRegionTemplate(std::vector<std::string> titles,
                          std::vector<TemplateQuestProperty> properties,
                          std::vector<TemplateQuestDescription> descriptions);
};

