//
// Created by michael on 06.11.15.
//

#pragma once

#include <unordered_map>
#include "../WeaverTypes.h"
#include "Node.h"

namespace weave {

    enum class EdgeType {
        DIRECT, TRANSITIVE
    };

    class Edge {
    public:
        Edge(ID nodeId1, ID nodeId2, EdgeType type);

        Edge(ID nodeId1, ID nodeId2, EdgeType type, ID questId);

        uint32_t Count(EdgeType type) const;

        ID Get(ID startId) const;

        ID GetNode1() const;

        ID GetNode2() const;

        std::unordered_set<ID> GetQuestIds() const;

        bool operator==(const Edge &other) const;

        bool operator<(const Edge &other) const;

    private:
        friend class WeaverGraph;

        ID id1;
        ID id2;
        std::unordered_map<EdgeType, uint32_t, EnumClassHash> types;
        std::unordered_set<ID> questIds;

        void addTypesFrom(const Edge &other);

        void addQuestIdsFrom(const Edge &other);
    };
}
