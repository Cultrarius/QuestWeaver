//
// Created by michael on 08.02.16.
//

#include <WeaverConfig.h>
#include <Template/Space/SpaceQuestTemplateFactory.h>
#include <Story/Space/CommonSpaceStoryFactory.h>
#include <World/Space/SpaceWorldModel.h>
#include <World/Space/SpaceNameGenerator.h>
#include "TestHelper.h"

using namespace weave;
using namespace std;

WeaverConfig TestHelper::CreateDebugConfig() {
    auto spaceFactory = unique_ptr<QuestTemplateFactory>(new SpaceQuestTemplateFactory());
    auto spaceStoryFactory = unique_ptr<StoryTemplateFactory>(new CommonSpaceStoryFactory());

    WeaverConfig config;
    config.randomStream = make_shared<RandomStream>(42);
    config.worldModel = unique_ptr<WorldModel>(new SpaceWorldModel());
    config.questTemplateFactories.push_back(std::move(spaceFactory));
    config.storyTemplateFactories.push_back(std::move(spaceStoryFactory));

    return config;
}
