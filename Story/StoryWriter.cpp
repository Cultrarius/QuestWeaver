//
// Created by michael on 21.12.15.
//

#include <Story/StoryWriter.h>

using namespace std;
using namespace weave;

StoryWriter::StoryWriter(std::shared_ptr<RandomStream> randomStream, const weave::QuestModel &model,
                         const weave::TemplateEngine &templateEngine) :
        rs(randomStream), model(model), templateEngine(templateEngine) {
}

void StoryWriter::initialize() const {
    if (isInitialized) {
        return;
    }
    isInitialized = true;
}

string StoryWriter::CreateStory(const WeaverGraph &graph) const {
    // TODO: the current template might also prove useful at this point to pick a more useful story

    initialize();
    return "In a far away galaxy a long time ago...\nThere were three litte piglets!";
}
