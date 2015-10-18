#include "cereal.h"
#include "QuestWeaver.h"
#include "QuestModel/Space/ExploreRegionQuest.h"
#include "World/Space/SpaceWorldModel.h"

using namespace std;
using namespace weave;
using namespace cereal;

int main() {
    QuestWeaver weaver(42);
    shared_ptr<Quest> newQuest = weaver.CreateNewQuest();
    cout << "New Quest created!" << endl;
    cout << "Title: " << newQuest->GetTitle() << endl;
    cout << "Description: " << newQuest->GetDescription() << endl;
    cout << "State: " << newQuest->GetState() << endl;

    shared_ptr<RandomStream> rs = make_shared<RandomStream>(11);
    SpaceWorldModel testModel(rs);
    auto location = testModel.CreateLocation();
    stringstream ss;
    {
        cereal::JSONOutputArchive outputArchive(ss);
        outputArchive(location);
    }

    cout << ss.str() << endl;

    {
        cereal::JSONInputArchive inputArchive(ss);
        inputArchive(location);
    }

    cout << location->GetId() << ", " << location->Z;

    return 0;
}
