//
// Created by michael on 16.02.16.
//

#include <Story/Space/AgentIntroStoryTemplate.h>

using namespace weave;
using namespace std;

weave::RawStory weave::AgentIntroStoryTemplate::CreateStory(std::map<std::string, std::shared_ptr<WorldEntity>> map,
                                                            const WeaverGraph &graph) const {
    return weave::RawStory(std::vector<StoryLine>());
}
