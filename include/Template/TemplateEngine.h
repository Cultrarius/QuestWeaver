//
// Created by michael on 10.08.15.
//

#pragma once

#include "../Core/WeaverUtils.h"
#include "../Core/WeaverTypes.h"
#include "TemplateFactory.h"
#include "Template.h"

namespace weave {
    class TemplateEngine {
    public:
        TemplateEngine(std::shared_ptr<RandomStream> randomStream, Directories dirs);

        std::shared_ptr<Template> GetTemplateForNewQuest();

        void RegisterTemplateFactory(std::shared_ptr<TemplateFactory> factory);

		void ChangeDirectories(Directories newDirs);

    private:
        std::vector<std::shared_ptr<TemplateFactory>> factories;

        std::shared_ptr<RandomStream> randomStream;

        Directories dirs;
    };
}
