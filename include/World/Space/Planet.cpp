//
// Created by michael on 16.03.16.
//

#include "Planet.h"

using namespace weave;
using namespace std;

std::string Planet::ToString() const {
    return Name;
}

Planet::Planet(ID id, float x, float y, int seed, std::string name) : WorldEntity(id), X(x), Y(y), Seed(seed),
                                                                      Name(name) {
}

Planet::Planet(float x, float y, int seed, std::string name) : Planet(NoID, x, y, seed, name) {
}

Planet::Planet() : Planet(0, 0, 0, 0, "") {
}

std::string Planet::GetType() const {
    return "planet";
}
