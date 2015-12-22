//
// Created by michael on 21.12.15.
//

#include "StoryWriter.h"

using namespace std;
using namespace weave;

string StoryWriter::CreateStory(const WeaverGraph &graph) const {
    initialize();
    return "";
}

void StoryWriter::initialize() const {
    if (isInitialized) {
        return;
    }
    isInitialized = true;
}

StoryWriter::StoryWriter(std::shared_ptr<RandomStream> randomStream, const weave::QuestModel &model,
                         const weave::TemplateEngine &templateEngine) :
        rs(randomStream), model(model), templateEngine(templateEngine) {
}

