//
// Created by michael on 29.01.16.
//

#include <Story/StoryTemplate.h>

using namespace weave;
using namespace std;

StoryTemplate::StoryTemplate() {

}

vector<string> StoryTemplate::GetRequiredEntities() const {
    vector<string> entities;
    entities.push_back("agent");
    return entities;
}
