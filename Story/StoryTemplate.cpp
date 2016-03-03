//
// Created by michael on 29.01.16.
//

#include <Story/StoryTemplate.h>

using namespace weave;
using namespace std;

StoryTemplate::StoryTemplate(vector<RawStoryLine> rawStoryLines) : rawLines(rawStoryLines) {
}

bool StoryTemplate::IsValid(const EntityMap &requiredEntities, const WeaverGraph &graph,
                            const WorldModel &worldModel) const {
    return true;
}

vector<StoryLine> StoryTemplate::createLinesSimple(const vector<ID> &idsForAllNuggets) const {
    vector<StoryLine> lines;
    for (auto rawLine : rawLines) {
        vector<NuggetOption> options;
        for (auto nuggetKey : rawLine.nuggets) {
            options.push_back(NuggetOption(nuggetKey, idsForAllNuggets));
        }
        lines.push_back(StoryLine(rawLine.prePart, options, rawLine.postPart));
    }
    return lines;
}

vector<StoryLine> StoryTemplate::createLinesSimple(const unordered_map<string, vector<ID>> &idsPerNugget) const {
    vector<StoryLine> lines;
    for (auto rawLine : rawLines) {
        vector<NuggetOption> options;
        for (auto nuggetKey : rawLine.nuggets) {
            auto iter = idsPerNugget.find(nuggetKey);
            if (iter == idsPerNugget.end()) {
                continue;
            }
            options.push_back(NuggetOption(nuggetKey, iter->second));
        }
        lines.push_back(StoryLine(rawLine.prePart, options, rawLine.postPart));
    }
    return lines;
}