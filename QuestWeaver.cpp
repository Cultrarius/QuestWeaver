//
// Created by michael on 10.08.15.
//

#include "QuestWeaver.h"

using namespace std;

QuestWeaver::QuestWeaver() {
    engine = unique_ptr<WeaverEngine>(new WeaverEngine());
}
