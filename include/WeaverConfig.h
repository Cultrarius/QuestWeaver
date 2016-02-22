//
// Created by michael on 18.01.16.
//

#pragma once

#include <World/WorldModel.h>
#include <Template/QuestTemplateFactory.h>
#include <Story/StoryTemplateFactory.h>

namespace weave {

    /*!
     * Holds the configuration data to create new QuestWeaver instances.
     *
     * @ingroup mainApi
     * @see QuestWeaver
     */
    struct WeaverConfig {
        /*!
         * The random seed used to create new quests.
         *
         * Quest Systems initialized with the same seed create the same
         * stream of quests (given the quests are initialized and completed the same way). If a random stream is
         * provided instead, the seed is ignored.
         */
        uint64_t seed = 0;

        /*!
         * The directories in which the templates and other files can be found in.
         * Also allows to define a mod directory which can overwrite the default directory for selected files.
         */
        Directories dirs;

        /*!
         * The world model is responsible for creating all quest entities and informs about world changes.
         * **MUST** be initialized by the caller before creating a new QuestWeaver instance.
         */
        std::unique_ptr<WorldModel> worldModel;

        /*!
         * The template factories used to create new quest templates.
         * At least one factory has to be registered before trying to create new quests.
         */
        std::vector<std::unique_ptr<QuestTemplateFactory>> questTemplateFactories;

        /*!
         * The template factories used to create the quest stories.
         * These are optional, but without them, the quests will be created without a backstory.
         */
        std::vector<std::unique_ptr<StoryTemplateFactory>> storyTemplateFactories;

        /*!
         * Defines how the quest texts such as objectives or description will be formatted.
         * This value cannot be changed later, so choose wisely.
         */
        FormatterType formatterType = FormatterType::TEXT;

        /*!
         *  **OPTIONAL**
         * Can be used to supply the quest system with a custom random number generator.
         *
         * If a nullptr is provided, the default RNG implementation with the provided seed will be used.
         * If a custom RNG is provided, the caller is responsible for the initial seeding of the stream.
         */
        std::shared_ptr<RandomStream> randomStream;

        /*!
         * Creates a new config using default values.
         * Please note that the config is **not valid** as long as no world model is assigned to it.
         */
        WeaverConfig() = default;

        /*!
         * Creates a new config using default values and the given seed.
         * Please note that the config is **not valid** as long as no world model is assigned to it.
         */
        explicit WeaverConfig(uint64_t seed) : seed(seed) { }
    };
}
