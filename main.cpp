#include <iostream>
#include "QuestWeaver.h"

using namespace std;
using namespace weave;

int main() {
    QuestWeaver weaver(42);
    Quest newQuest = weaver.CreateNewQuest();
    cout << "New Quest created!" << endl;
    cout << "Title: " << newQuest.getTitle() << endl;
    cout << "Description: " << newQuest.getDescription() << endl;
    cout << "State: " << newQuest.getState() << endl;
    return 0;
}
