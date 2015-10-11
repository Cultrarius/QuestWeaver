#include <iostream>
#include "QuestWeaver.h"

using namespace std;
using namespace weave;

int main() {
    QuestWeaver weaver;
    Quest newQuest = weaver.CreateNewQuest();
    cout << "Quest: " << newQuest.getTitle() << endl;
    return 0;
}
