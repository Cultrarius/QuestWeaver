//
// Created by michael on 06.11.15.
//

#include "Edge.h"

weave::Edge::Edge(weave::ID nodeId1, weave::ID nodeId2, weave::EdgeType type) {
    id1 = nodeId1;
    id2 = nodeId2;
    types.push_back(type);
}

const std::vector<weave::EdgeType> &weave::Edge::GetTypes() const {
    return types;
}

weave::ID weave::Edge::Get(weave::ID startId) const {
    return (startId == id1) ? id2 : ((startId == id2) ? id1 : 0);
}

bool weave::Edge::operator==(const weave::Edge &other) const {
    return (id1 == other.id1 && id2 == other.id2) || (id1 == other.id2 && id2 == other.id1);
}

void weave::Edge::addTypesFrom(const weave::Edge &other) {
    if (!this->operator==(other)) {
        throw ContractFailedException("unable to combine edges from different nodes!");
    }
    for (auto type : other.types) {
        types.push_back(type);
    }
}

bool weave::Edge::operator<(const weave::Edge &other) const {
    return (id1 + id2) < (other.id1 + other.id2);
}
