//
// Created by michael on 06.11.15.
//

#include <algorithm>
#include <Core/Graph/Edge.h>

using namespace std;
using namespace weave;

Edge::Edge(ID nodeId1, ID nodeId2, EdgeType type) : Edge(nodeId1, nodeId2, type, 0) {
}

Edge::Edge(ID nodeId1, ID nodeId2, EdgeType type, ID questId) {
    if (nodeId1 == nodeId2) {
        string errorMsg = "Can not create an edge between the same nodes!";
        Logger::Error(ContractFailedException(errorMsg));
        return;
    }
    id1 = min(nodeId1, nodeId2);
    id2 = max(nodeId1, nodeId2);
    types[type] = 1;
    if (questId != 0) {
        questIds.insert(questId);
    }
}

ID Edge::Get(ID startId) const {
    return (startId == id1) ? id2 : ((startId == id2) ? id1 : 0);
}

bool Edge::operator==(const Edge &other) const {
    return (id1 == other.id1 && id2 == other.id2) || (id1 == other.id2 && id2 == other.id1);
}

void Edge::addTypesFrom(const Edge &other) {
    if (!this->operator==(other)) {
        string errorMsg = "unable to combine edges from different nodes!";
        Logger::Error(ContractFailedException(errorMsg));
        return;
    }
    for (auto type : other.types) {
        types[type.first] += type.second;
    }
}

bool Edge::operator<(const Edge &other) const {
    return id1 < other.id1 || (id1 == other.id1 && id2 < other.id2);
}

uint32_t Edge::Count(EdgeType type) const {
    auto it = types.find(type);
    if (it == types.end()) {
        return 0;
    }
    return it->second;
}

ID Edge::GetNode1() const {
    return id1;
}

ID Edge::GetNode2() const {
    return id2;
}

std::unordered_set<ID> Edge::GetQuestIds() const {
    return questIds;
}

void Edge::addQuestIdsFrom(const Edge &other) {
    if (!this->operator==(other)) {
        string errorMsg = "unable to combine edges from different nodes!";
        Logger::Error(ContractFailedException(errorMsg));
        return;
    }
    for (ID id : other.questIds) {
        questIds.insert(id);
    }
}
