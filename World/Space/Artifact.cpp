//
// Created by michael on 03.05.16.
//

#include <World/Space/Artifact.h>

using namespace weave;
using namespace std;

const string Artifact::Type = "artifact";

std::string Artifact::ToString() const noexcept {
    return Name;
}

Artifact::Artifact(ID id, std::string name) : WorldEntity(id), Name(name) {
}

Artifact::Artifact(std::string name) : Artifact(NoID, name) {
}

Artifact::Artifact() : Artifact(0, "") {
}

std::string Artifact::GetType() const noexcept {
    return Type;
}
