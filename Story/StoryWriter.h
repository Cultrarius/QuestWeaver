//
// Created by michael on 21.12.15.
//


#include "../Core/WeaverUtils.h"
#include "../Core/Graph/WeaverGraph.h"
#include "../QuestModel/QuestModel.h"

namespace weave {
    class StoryWriter {
    public:
        explicit StoryWriter(std::shared_ptr<RandomStream> randomStream);

        std::string CreateStory(const weave::WeaverGraph &graph, const weave::QuestModel &model);

    private:
        std::shared_ptr<RandomStream> rs;
    };
}
