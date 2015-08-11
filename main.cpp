#include <iostream>
#include "QuestWeaver.h"

using namespace std;

int main() {
    QuestWeaver weaver;

    cout << "Size: " << weaver.getActiveQuests().size() << endl;
    return 0;
}