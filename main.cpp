#include <cereal/cereal.hpp>
#include <cereal/archives/portable_binary.hpp>
#include <cereal/archives/json.hpp>
#include "QuestWeaver.h"

using namespace std;
using namespace weave;
using namespace cereal;

int main() {
    QuestWeaver weaver(42);
    std::shared_ptr<Quest> newQuest = weaver.CreateNewQuest();
    cout << "New Quest created!" << endl;
    cout << "Title: " << newQuest->GetTitle() << endl;
    cout << "Description: " << newQuest->GetDescription() << endl;
    cout << "State: " << newQuest->GetState() << endl;

    stringstream ss;
    {
        cereal::JSONOutputArchive outputArchive(ss);
        outputArchive(*newQuest);
    }
    cout << ss.str();

    return 0;
}
