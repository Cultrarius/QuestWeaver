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
    ModelAction locationToAdd(ActionType::CREATE, location);
    vector<ModelAction> modelActions;
    modelActions.push_back(locationToAdd);
    testModel.Execute(modelActions);

    stringstream ss;
    {
        cereal::JSONOutputArchive outputArchive(ss);
        outputArchive(testModel);
    }

    cout << ss.str() << endl;

    shared_ptr<RandomStream> rs2 = make_shared<RandomStream>(21);
    SpaceWorldModel deserializedModel(rs2);
    {
        cereal::JSONInputArchive inputArchive(ss);
        inputArchive(deserializedModel);
    }

    cout << testModel.CreateLocation()->X << " ";
    cout << deserializedModel.CreateLocation()->X << endl;

    return 0;
}
