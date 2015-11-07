//
// Created by michael on 06.11.15.
//

#include "Edge.h"

using namespace std;
using namespace weave;

Edge::Edge(ID nodeId1, ID nodeId2, EdgeType type) {
    if (nodeId1 == nodeId2) {
        throw ContractFailedException("Can not create an edge between the same nodes!");
    }
    id1 = nodeId1;
    id2 = nodeId2;
    types[type] = 1;
}

ID Edge::Get(ID startId) const {
    return (startId == id1) ? id2 : ((startId == id2) ? id1 : 0);
}

bool Edge::operator==(const Edge &other) const {
    return (id1 == other.id1 && id2 == other.id2) || (id1 == other.id2 && id2 == other.id1);
}

void Edge::addTypesFrom(const Edge &other) {
    if (!this->operator==(other)) {
        throw ContractFailedException("unable to combine edges from different nodes!");
    }
    for (auto type : other.types) {
        types[type.first] += type.second;
    }
}

bool Edge::operator<(const Edge &other) const {
    return (id1 + id2) < (other.id1 + other.id2);
}

uint32_t Edge::Count(EdgeType type) const {
    auto it = types.find(type);
    if (it == types.end()) {
        return 0;
    }
    return it->second;
}
