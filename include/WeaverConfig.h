//
// Created by michael on 18.01.16.
//

#pragma once

#include <World/WorldModel.h>
#include <Template/QuestTemplateFactory.h>
#include <Story/StoryTemplateFactory.h>

namespace weave {

    struct WeaverConfig {
        /*
         * The random seed used to create new quests. Quest Systems initialized with the same seed create the same
         * stream of quests (given the quests are initialized and completed the same way).
         */
        uint64_t seed = 0;

        /*
         * The directories in which the templates and other files can be found in. Also allows to define a mod directory
         * which can overwrite the default directory for selected files.
         */
        Directories dirs;

        /*
         * The world model is responsible for creating all quest entities and informs about world changes.
         * MUST be initialized by the caller before creating a new QuestWeaver instance.
         */
        WorldModel *worldModel = nullptr;

        /*
         * The template factories used to create new quest templates.
         * At least one factory has to be registered before trying to create new quests.
         */
        std::vector<std::shared_ptr<QuestTemplateFactory>> questTemplateFactories;

        /*
         * The template factories used to create the quest stories.
         * These are optional, but without them, the quests will be created without a backstory.
         */
        std::vector<std::shared_ptr<StoryTemplateFactory>> storyTemplateFactories;

        /*
         * The formatter type defines how the quest texts such as objectives or description will be formatted.
         * This value cannot be changed later, so choose wisely.
         */
        FormatterType formatterType = FormatterType::TEXT;

        /*
         * True if used for testing and debugging.
         */
        bool debug = true;

        /*
         * *OPTIONAL*
         *
         * Can be used to supply the quest system with a custom random number generator.
         * If a nullptr is provided, the default RNG implementation will be used.
         * Please note that the quest system takes ownership over the random stream,
         * so it will delete the stream once it is destroyed.
         */
        RandomStream *randomStream = nullptr;

        WeaverConfig() = default;

        explicit WeaverConfig(uint64_t seed) : seed(seed) { }
    };
}
