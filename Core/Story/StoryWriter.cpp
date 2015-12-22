//
// Created by michael on 21.12.15.
//

#include "StoryWriter.h"

using namespace std;
using namespace weave;

string StoryWriter::CreateStory(const WeaverGraph &graph, const QuestModel &model) {
    return "";
}

StoryWriter::StoryWriter(std::shared_ptr<RandomStream> randomStream) : rs(randomStream) {
}
