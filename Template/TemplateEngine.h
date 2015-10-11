//
// Created by michael on 10.08.15.
//

#pragma once

#include "Template.h"
#include "TemplateFactory.h"
#include "../Core/WeaverUtils.h"
#include <memory>

namespace weave {
    class TemplateEngine {
    public:
        std::shared_ptr<Template> GetTemplateForNewQuest(weave::RandomStream *randomStream);

        void RegisterTemplateFactory(std::shared_ptr<TemplateFactory> factory);

    private:
        std::vector<std::shared_ptr<TemplateFactory>> factories;
    };
}
