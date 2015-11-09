//
// Created by michael on 07.11.15.
//

#pragma once

#include <unordered_map>
#include "WeaverTypes.h"
#include "Graph/WeaverGraph.h"

namespace weave {
    struct AnalyzerParameters {
        float optionalNodePenalty = -100;
        float previousQuestBonus = 50;

        template<class Archive>
        void serialize(Archive &ar) {
            ar(CEREAL_NVP(optionalNodePenalty), CEREAL_NVP(previousQuestBonus));
        }
    };

    class GraphAction {
    public:
        GraphAction(bool isActivate, const Node &node) : isActivate(isActivate), node(node) { }

        const Node &GetNode() const;

        bool IsActivate() const;

        bool operator==(const GraphAction &other) const;

        bool operator<(const GraphAction &other) const;

    private:
        bool isActivate;
        const Node node;
    };

    class GraphAnalyzer {
    public:
        static std::unordered_map<std::string, Node> SolveGraph(WeaverGraph *graph,
                                                                std::shared_ptr<RandomStream> randomStream);

    private:
        static bool fillActionMap(WeaverGraph *graph, std::map<GraphAction, float> *map);

        static float getGraphScore(WeaverGraph *graph);

        static float tryMandatoryNode(WeaverGraph *graph, const std::string &group, const Node &node);

        static float tryOptionalNode(WeaverGraph *graph, const std::string &group, const Node &node, bool isActive,
                                     bool hasActiveNode);
    };
}
