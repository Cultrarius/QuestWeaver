#include <iostream>
#include "QuestWeaver.h"

using namespace std;

int main() {
    QuestWeaver weaver;
    Quest newQuest = weaver.CreateNewQuest();
    cout << "Quest: " << newQuest.getTitle() << endl;
    return 0;
}