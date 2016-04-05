//
// Created by michael on 10.08.15.
//

#pragma once

#include "../Core/WeaverUtils.h"
#include "../Core/WeaverTypes.h"
#include "QuestTemplateFactory.h"
#include "QuestTemplate.h"

namespace weave {

    /*!
     * @ingroup internal
     */
    class TemplateEngine {
    public:
        TemplateEngine(std::shared_ptr<RandomStream> randomStream, Directories dirs, FormatterType format);

        std::vector<std::shared_ptr<QuestTemplate>> GetTemplatesForNewQuest(const WorldModel &worldModel,
                                                                            const QuestModel &questModel);

        void RegisterTemplateFactory(std::unique_ptr<QuestTemplateFactory> factory);

        void ChangeDirectories(Directories newDirs);

        FormatterType GetFormat() const;

    private:
        std::vector<std::unique_ptr<QuestTemplateFactory>> factories;

        std::shared_ptr<RandomStream> randomStream;

        Directories dirs;

        FormatterType format;
    };
}
