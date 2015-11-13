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
        float transitiveQuestBonus = 10;

        template<class Archive>
        void serialize(Archive &ar) {
            ar(CEREAL_NVP(optionalNodePenalty), CEREAL_NVP(previousQuestBonus));
        }
    };

    class GraphAction {
    public:
        GraphAction(bool isActivate, const Node &node);

        explicit GraphAction(std::map<Node, bool> nodeActivations) : nodeActivations(nodeActivations) { }

        void Apply(WeaverGraph *graph) const;

        bool operator==(const GraphAction &other) const;

        bool operator<(const GraphAction &other) const;

    private:
        std::map<Node, bool> nodeActivations;
    };

    class GraphAnalyzer {
    public:
        static std::unordered_map<std::string, Node> SolveGraph(WeaverGraph *graph,
                                                                std::shared_ptr<RandomStream> randomStream);

        static std::unordered_map<std::string, Node> SolveGraph(WeaverGraph *graph,
                                                                std::shared_ptr<RandomStream> randomStream,
                                                                AnalyzerParameters parameters);

    private:
        static bool fillActionMap(WeaverGraph *graph, std::map<GraphAction, float> *map,
                                  const AnalyzerParameters &param, std::shared_ptr<RandomStream> rs);

        static float getGraphScore(WeaverGraph *graph, const AnalyzerParameters &param);

        static void getSingleNodeActions(WeaverGraph *graph, std::map<GraphAction, float> *map,
                                         const AnalyzerParameters &param);

        static void getEdgeActions(WeaverGraph *graph, std::map<GraphAction, float> *map,
                                   const AnalyzerParameters &param, std::shared_ptr<RandomStream> rs);
    };
}
