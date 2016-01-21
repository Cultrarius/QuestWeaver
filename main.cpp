#include <Template/Space/SpaceQuestTemplateFactory.h>
#include "QuestWeaver.h"

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

    stringstream ss;
    weaver.serialize(ss, StreamType::BINARY);
    cout << ss.str() << endl;

    cout << "Hey!" << endl;

    QuestWeaver deserialized = QuestWeaver::deserialize(ss, StreamType::BINARY);
    shared_ptr<TemplateFactory> factory = make_shared<SpaceQuestTemplateFactory>();;
    deserialized.RegisterTemplateFactory(factory);
    cout << "Woot!" << endl;

    shared_ptr<Quest> desQuest = deserialized.GetQuest(newQuest->GetId());

    cout << "Title: " << desQuest->GetTitle() << endl;
    cout << "Description: " << desQuest->GetDescription() << endl;

    newQuest = deserialized.CreateNewQuest();
    cout << "New Quest created!" << endl;
    cout << "Title: " << newQuest->GetTitle() << endl;
    cout << "Description: " << newQuest->GetDescription() << endl;

//    shared_ptr<RandomStream> rs = make_shared<RandomStream>(11);
//    SpaceWorldModel testModel(rs);
//    auto location = testModel.CreateLocation();
//    auto solar = make_shared<SolarSystem>();
//    WorldModelAction locationToAdd(WorldActionType::CREATE, solar);
//    vector<WorldModelAction> modelActions;
//    modelActions.push_back(locationToAdd);
//    testModel.Execute(modelActions);
//
//    testModel.GetMetaData(1).SetValue("Bla", 9);
//    testModel.GetMetaData(1).SetValue("Whoop", 42);
//
//    stringstream ss;
//    {
//        cereal::JSONOutputArchive outputArchive(ss);
//        outputArchive(testModel);
//    }
//
//    cout << ss.str() << endl;
//
//    shared_ptr<RandomStream> rs2 = make_shared<RandomStream>(21);
//    SpaceWorldModel deserializedModel(rs2);
//    {
//        cereal::JSONInputArchive inputArchive(ss);
//        inputArchive(deserializedModel);
//    }
//
//    cout << "Has: " << deserializedModel.GetMetaData(1).HasValue("Bla") << endl;
//    cout << "Val: " << deserializedModel.GetMetaData(1).GetValue("Bla") << endl;
//    cout << testModel.CreateLocation()->X << " ";
//    cout << deserializedModel.CreateLocation()->X << endl;

    return 0;
}
