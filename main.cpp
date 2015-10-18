#include "cereal.h"
#include "QuestWeaver.h"
#include "QuestModel/Space/ExploreRegionQuest.h"

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

    QuestModel testModel;
    shared_ptr<Quest> registeredQuest = testModel.registerQuest(*newQuest);
    stringstream ss;
    {
        cereal::JSONOutputArchive outputArchive(ss);
        outputArchive(testModel);
    }
    cout << ss.str();

    return 0;
}
