//
// Created by michael on 15.08.15.
//

#pragma once


#include "../TemplateFactory.h"

class SpaceQuestTemplateFactory : public TemplateFactory {
public:
    SpaceQuestTemplateFactory();

    Template CreateTemplate(std::string templateKey);
};


