//
// Created by michael on 21.12.15.
//

#include <Story/StoryWriter.h>

using namespace std;
using namespace weave;

StoryWriter::StoryWriter(shared_ptr<RandomStream> randomStream, const QuestModel &model,
                         const TemplateEngine &templateEngine, const Directories &dirs) :
        rs(randomStream), model(model), templateEngine(templateEngine), dirs(dirs) {
}

void StoryWriter::initialize() const {
    if (isInitialized) {
        return;
    }
    isInitialized = true;
    for (auto factory : factories) {
        factory->initialize();
    }
    readNuggets();
}

void StoryWriter::readNuggets() const {
    nuggets.clear();
    set<string> nuggetFolders;
    for (auto factory : factories) {
        nuggetFolders.insert(factory->GetNuggetFolder());
    }

    for (string folder : nuggetFolders) {
        string file = folder;
        file += "/Nuggets.st";
        Json::Value root = readJsonFromFile(file.c_str(), dirs);
    }
}

string StoryWriter::CreateStory(const WeaverGraph &graph) const {
    // TODO: the current template might also prove useful at this point to pick a more useful story

    initialize();
    return "In a far away galaxy a long time ago...\nThere were three little piglets!";
}

void StoryWriter::ChangeDirectories(const Directories &newDirs) {
    dirs = newDirs;
    isInitialized = false;
    for (auto factory : factories) {
        factory->dirs = dirs;
        factory->isInitialized = false;
    }
}