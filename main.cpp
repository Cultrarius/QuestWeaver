#include <iostream>
#include "QuestWeaver.h"

using namespace std;
using namespace weave;

int main() {
    QuestWeaver weaver(42);
    std::shared_ptr<Quest> newQuest = weaver.CreateNewQuest();
    cout << "New Quest created!" << endl;
    cout << "Title: " << newQuest->GetTitle() << endl;
    cout << "Description: " << newQuest->GetDescription() << endl;
    cout << "State: " << newQuest->GetState() << endl;
    return 0;
}
