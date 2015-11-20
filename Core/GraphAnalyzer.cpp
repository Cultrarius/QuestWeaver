//
// Created by michael on 07.11.15.
//

#include <assert.h>
#include "GraphAnalyzer.h"

using namespace std;
using namespace weave;

unordered_map<string, Node> GraphAnalyzer::SolveGraph(weave::WeaverGraph *graph,
                                                      shared_ptr<RandomStream> rs,
                                                      AnalyzerParameters param) {
    initializeMandatoryNodes(graph, rs);

    map<GraphAction, float> actionMap;
    while (fillActionMap(graph, &actionMap, param, rs)) {
        // search for a random action from the action map
        // actions with a higher score are more likely to be chosen
        float minScore = 0;
        float maxScore = 0;
        for (auto pair : actionMap) {
            minScore = minScore < pair.second ? minScore : pair.second;
            maxScore = maxScore > pair.second ? maxScore : pair.second;
        }

        float currentMax = 0;
        const GraphAction *action = nullptr;
        for (auto pair : actionMap) {
            float percent = (pair.second - minScore) / maxScore;
            float random = rs->GetIntInRange(0, 9000) * percent + rs->GetIntInRange(0, 1000);
            if (random >= currentMax) {
                currentMax = random;
                action = &pair.first;
            }
        }
        assert(action != nullptr);
        action->Apply(graph);
        actionMap.clear();
    }

    unordered_map<string, Node> results;
    for (auto node : graph->GetActiveNodes()) {
        results[node.GetGroup()] = node;
    }
    return results;
}

void GraphAnalyzer::initializeMandatoryNodes(WeaverGraph *graph, shared_ptr<RandomStream> rs) {
    // activate random mandatory nodes, so the graph is in a consistent state
    for (auto group : graph->GetMandatoryGroups()) {
        auto nodes = graph->GetNodes(group);
        auto index = rs->GetRandomIndex(nodes.size());
        graph->ActivateNode(nodes.at(index));
    }
}

bool GraphAction::operator==(const GraphAction &other) const {
    auto otherActivations = other.nodeActivations;
    for (auto pair : nodeActivations) {
        auto iter = otherActivations.find(pair.first);
        if (iter == otherActivations.end() || iter->second != pair.second) {
            return false;
        }
    }
    return true;
}

bool GraphAction::operator<(const GraphAction &other) const {
    auto otherActivations = other.nodeActivations;
    return (nodeActivations.size() < otherActivations.size());
}

bool GraphAnalyzer::fillActionMap(WeaverGraph *graph, map<GraphAction, float> *map, const AnalyzerParameters &param,
                                  shared_ptr<RandomStream> rs) {
    getSingleNodeActions(graph, map, param);
    getEdgeActions(graph, map, param, rs);
    return !map->empty();
}

float GraphAnalyzer::getGraphScore(WeaverGraph *graph, const AnalyzerParameters &param) {
    float score = 0;
    auto mandatory = graph->GetMandatoryGroups();
    unordered_set<ID> activeNodeIds;

    for (auto node : graph->GetActiveNodes()) {
        activeNodeIds.insert(node.GetId());
        if (mandatory.find(node.GetGroup()) == mandatory.end()) {
            score += param.optionalNodePenalty;
        }
    }
    for (auto edge : graph->GetEdges()) {
        if ((activeNodeIds.find(edge.GetNode1()) != activeNodeIds.end()) &&
            (activeNodeIds.find(edge.GetNode2()) != activeNodeIds.end())) {
            score += edge.Count(EdgeType::DIRECT) * param.previousQuestBonus;
            score += edge.Count(EdgeType::TRANSITIVE) * param.transitiveQuestBonus;
        }
    }
    return score;
}

std::unordered_map<std::string, Node> GraphAnalyzer::SolveGraph(WeaverGraph *graph,
                                                                std::shared_ptr<RandomStream> randomStream) {
    return SolveGraph(graph, randomStream, AnalyzerParameters());
}

GraphAction::GraphAction(bool isActivate, const Node &node) {
    nodeActivations[node] = isActivate;
}

void GraphAction::Apply(WeaverGraph *graph) const {
    for (auto pair : nodeActivations) {
        if (pair.second) {
            graph->ActivateNode(pair.first);
        } else {
            graph->DeactivateNode(pair.first);
        }
    }
}

void GraphAnalyzer::getSingleNodeActions(WeaverGraph *graph, std::map<GraphAction, float> *map,
                                         const AnalyzerParameters &param) {
    float startScore = getGraphScore(graph, param);
    auto mandatoryGroups = graph->GetMandatoryGroups();
    for (string group : graph->GetGroups()) {
        bool isMandatory = mandatoryGroups.find(group) != mandatoryGroups.end();
        for (auto &node : graph->GetNodes(group)) {
            // this is not the best solution for performance, but has better readability
            WeaverGraph graphCopy = *graph;
            auto isActive = graph->IsNodeActive(node);

            GraphAction action(!isActive, node);
            if (isMandatory && isActive) {
                // we can not deactivate nodes in mandatory groups
                continue;
            }
            action.Apply(&graphCopy);
            float actionScore = getGraphScore(&graphCopy, param);

            if (actionScore > startScore) {
                (*map)[action] = actionScore;
            }
        }
    }
}

void GraphAnalyzer::getEdgeActions(WeaverGraph *graph, std::map<GraphAction, float> *map,
                                   const AnalyzerParameters &param, shared_ptr<RandomStream> rs) {
    float startScore = getGraphScore(graph, param);
    for (Edge edge : graph->GetEdges()) {
        auto nodes1 = graph->GetNodesWithId(edge.GetNode1());
        auto nodes2 = graph->GetNodesWithId(edge.GetNode2());
        if (nodes1.empty() || nodes2.empty()) {
            continue;
        }
        auto node1 = nodes1.at(rs->GetRandomIndex(nodes1.size()));
        auto node2 = nodes2.at(rs->GetRandomIndex(nodes2.size()));
        std::map<Node, bool> activeNodes;
        activeNodes[node1] = true;
        activeNodes[node2] = true;

        WeaverGraph graphCopy = *graph;
        GraphAction action(activeNodes);
        action.Apply(&graphCopy);
        float actionScore = getGraphScore(&graphCopy, param);

        if (actionScore > startScore) {
            (*map)[action] = actionScore;
        }
    }
}
