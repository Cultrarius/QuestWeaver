//
// Created by michael on 15.03.16.
//

#include "SpaceNameGenerator.h"
#include <algorithm>
#include <Core/NameGen/TokenNameGenerator.h>

using namespace weave;
using namespace std;

std::string weave::SpaceNameGenerator::CreateName(NameType nameType, std::shared_ptr<RandomStream> random) {

    string pattern = "iV";

//    for (uint64_t i = 0; i < random->GetULongInRange(min(maxParts, (uint64_t) 2), maxParts); i++) {
//        if (nameType == NameType::DARK_PERSON) {
//            name << darkParts[random->GetRandomIndex(darkParts.size())];
//        } else if (nameType == NameType::DARK_THING) {
//            name << darkParts[random->GetRandomIndex(darkParts.size())];
//        } else {
//            throw ContractFailedException("Unknown name type.");
//        }
//    }
    TokenNameGenerator nameGenerator(pattern);
    std::cerr << "> combinations = " << nameGenerator.combinations() << "\n";
    return nameGenerator.toString(random);
}

