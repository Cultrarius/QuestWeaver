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

        uint32_t Count(EdgeType type) const;

        ID Get(ID startId) const;

        bool operator==(const Edge &other) const;

        bool operator<(const Edge &other) const;

    private:
        friend class WeaverGraph;

        ID id1;
        ID id2;
        std::unordered_map<EdgeType, uint32_t, EnumClassHash> types;

        void addTypesFrom(const Edge &other);
    };
}
