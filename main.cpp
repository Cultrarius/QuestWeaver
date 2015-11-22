#include "cereal.h"
#include "QuestWeaver.h"
#include "QuestModel/Space/ExploreRegionQuest.h"
#include "World/Space/SpaceWorldModel.h"
#include "World/Space/SolarSystem.h"

using namespace std;
using namespace weave;
using namespace cereal;

int main() {
    QuestWeaver weaver(42);
    shared_ptr<Quest> newQuest = weaver.CreateNewQuest();
    cout << "New Quest created!" << endl;
    cout << "Title: " << newQuest->GetTitle() << endl;
    cout << "Description: " << newQuest->GetDescription() << endl;

    weaver.Tick(1);

    shared_ptr<RandomStream> rs = make_shared<RandomStream>(11);
    SpaceWorldModel testModel(rs);
    auto location = testModel.CreateLocation();
    auto solar = make_shared<SolarSystem>();
    WorldModelAction locationToAdd(WorldActionType::CREATE, solar);
    vector<WorldModelAction> modelActions;
    modelActions.push_back(locationToAdd);
    testModel.Execute(modelActions);

    testModel.GetMetaData(1).SetValue("Bla", 9);
    testModel.GetMetaData(1).SetValue("Whoop", 42);

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

    cout << "Has: " << deserializedModel.GetMetaData(1).HasValue("Bla") << endl;
    cout << "Val: " << deserializedModel.GetMetaData(1).GetValue("Bla") << endl;
    cout << testModel.CreateLocation()->X << " ";
    cout << deserializedModel.CreateLocation()->X << endl;

    return 0;
}
