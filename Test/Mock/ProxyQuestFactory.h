//
// Created by michael on 12.03.16.
//

#pragma once

#include <Template/Space/SpaceQuestTemplateFactory.h>
#include "Template/QuestTemplateFactory.h"

namespace weave {
    class ProxyQuestFactory : public SpaceQuestTemplateFactory {
    public:
        explicit ProxyQuestFactory(std::string key) {
            this->keys = {key};
        }

        std::vector<std::string> GetTemplateKeys() override {
            QuestTemplateFactory::GetTemplateKeys();
            return keys;
        }

    private:
        std::vector<std::string> keys;
    };
}
