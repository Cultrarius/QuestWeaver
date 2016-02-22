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
    auto spaceFactory = make_unique<SpaceQuestTemplateFactory>();
    auto spaceStoryFactory = make_shared<CommonSpaceStoryFactory>();

    WeaverConfig config;
    config.randomStream = std::make_shared<RandomStream>(42);
    config.worldModel = std::make_unique<SpaceWorldModel>();
    config.questTemplateFactories.push_back(std::move(spaceFactory));
    config.storyTemplateFactories.push_back(spaceStoryFactory);

    return config;
}
