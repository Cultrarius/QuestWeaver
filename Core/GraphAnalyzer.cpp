//
// Created by michael on 07.11.15.
//

#include "GraphAnalyzer.h"

using namespace std;
using namespace weave;

unordered_map<string, Node> GraphAnalyzer::SolveGraph(weave::WeaverGraph *graph,
                                                      shared_ptr<RandomStream> rs) {
    // activate random mandatory nodes, so the graph is in a consistent state
    for (auto group : graph->GetMandatoryGroups()) {
        auto nodes = graph->GetNodes(group);
        int index = rs->GetRandomIndex(nodes.size());
        graph->ActivateNode(nodes[index]);
    }

    map<GraphAction, float> actionMap;
    while (fillActionMap(graph, &actionMap)) {
        auto action = actionMap.begin()->first;
        if (action.IsActivate()) {
            graph->ActivateNode(action.GetNode());
        } else {
            graph->DeactivateNode(action.GetNode());
        }
    }

    unordered_map<string, Node> results;
    for (auto node : graph->GetActiveNodes()) {
        results[node.GetGroup()] = node;
    }
    return results;
}

const Node &GraphAction::GetNode() const {
    return node;
}

bool GraphAction::IsActivate() const {
    return isActivate;
}

bool GraphAction::operator==(const GraphAction &other) const {
    return node == other.node && isActivate == other.isActivate;
}

bool GraphAction::operator<(const GraphAction &other) const {
    return node < other.node && isActivate == other.isActivate;
}

bool GraphAnalyzer::fillActionMap(WeaverGraph *graph,
                                  map<GraphAction, float> *map) {
    float startScore = getGraphScore(graph);

    auto mandatoryGroups = graph->GetMandatoryGroups();
    for (string group : graph->GetGroups()) {
        bool isMandatory = mandatoryGroups.find(group) != mandatoryGroups.end();
        for (auto &node : graph->GetNodes(group)) {
            auto isActive = graph->IsNodeActive(node);
            bool hasActiveNode = isActive || graph->HasActiveNode(group);

            GraphAction action(!isActive, node);
            float actionScore = 0;
            if (isMandatory) {
                if (isActive) {
                    // we can not deactivate nodes in mandatory groups
                    continue;
                }
                actionScore = tryMandatoryNode(graph, group, node);
            } else {
                actionScore = tryOptionalNode(graph, group, node, isActive, hasActiveNode);
            }

            if (actionScore > startScore) {
                (*map)[action] = actionScore;
            }
        }
    }

    return !map->empty();
}

float GraphAnalyzer::tryOptionalNode(WeaverGraph *graph, const string &group, const Node &node, bool isActive,
                                     bool hasActiveNode) {
    float actionScore = 0;
    if (isActive) {
        graph->DeactivateNode(node);
        actionScore = getGraphScore(graph);
        graph->ActivateNode(node);
    } else {
        graph->ActivateNode(node);
        actionScore = getGraphScore(graph);
        if (hasActiveNode) {
            graph->DeactivateNode(node);
        } else {
            graph->ActivateNode(graph->GetActiveNode(group));
        }
    }
    return actionScore;
}

float GraphAnalyzer::tryMandatoryNode(WeaverGraph *graph, const string &group, const Node &node) {
    graph->ActivateNode(node);
    float actionScore = getGraphScore(graph);
    graph->ActivateNode(graph->GetActiveNode(group));
    return actionScore;
}

float GraphAnalyzer::getGraphScore(WeaverGraph *graph) {
    return 0;
}
