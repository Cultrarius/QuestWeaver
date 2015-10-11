#include <iostream>
#include "QuestWeaver.h"

using namespace std;
using namespace weave;

int main() {
    QuestWeaver weaver(42);
    Quest newQuest = weaver.CreateNewQuest();
    cout << "Quest: " << newQuest.getTitle() << endl;
    return 0;
}
