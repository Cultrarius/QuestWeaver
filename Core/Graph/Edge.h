//
// Created by michael on 06.11.15.
//

#pragma once

#include "../WeaverTypes.h"
#include "Node.h"

namespace weave {

    enum class EdgeType {
        DIRECT, TRANSITIVE
    };

    class Edge {
    public:
        Edge(ID nodeId1, ID nodeId2, EdgeType type);

        const std::vector<EdgeType> &GetTypes() const;

        ID Get(ID startId) const;

        bool operator==(const Edge &other) const;

        bool operator<(const Edge &other) const;

    private:
        friend class WeaverGraph;

        ID id1;
        ID id2;
        std::vector<EdgeType> types;

        void addTypesFrom(const Edge &other);
    };
}
