//
// Created by michael on 08.02.16.
//

#include <WeaverConfig.h>
#include <Template/Space/SpaceQuestTemplateFactory.h>
#include <Story/Space/CommonSpaceStoryFactory.h>
#include <World/Space/SpaceWorldModel.h>
#include "TestHelper.h"

using namespace weave;
using namespace std;

WeaverConfig TestHelper::CreateDebugConfig() {
    auto spaceFactory = std::make_shared<SpaceQuestTemplateFactory>();
    auto spaceStoryFactory = std::make_shared<CommonSpaceStoryFactory>();

    WeaverConfig config;
    config.randomStream = new RandomStream(42);
    config.worldModel = std::make_unique<SpaceWorldModel>();
    config.questTemplateFactories.push_back(spaceFactory);
    config.storyTemplateFactories.push_back(spaceStoryFactory);

    return config;
}
