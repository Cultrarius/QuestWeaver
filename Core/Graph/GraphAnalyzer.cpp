//
// Created by michael on 07.11.15.
//

#include <assert.h>
#include <Core/Graph/GraphAnalyzer.h>

using namespace std;
using namespace weave;

unordered_map<string, Node> GraphAnalyzer::SolveGraph(weave::WeaverGraph *graph,
                                                      shared_ptr<RandomStream> rs,
                                                      AnalyzerParameters param) {
    map<GraphAction, float> actionMap;
    int iterations = 1;
    while (fillActionMap(*graph, &actionMap, param, rs)) {
        assert(!actionMap.empty());

        // search for a random action from the action map
        // actions with a higher score are more likely to be chosen
        float minScore = 0;
        float maxScore = 0;
        for (auto pair : actionMap) {
            minScore = minScore < pair.second ? minScore : pair.second;
            maxScore = maxScore > pair.second ? maxScore : pair.second;
        }

        float currentMax = 0;
        GraphAction action = actionMap.begin()->first;
        for (auto pair : actionMap) {
            float percent = (pair.second - minScore) / maxScore;
            float random = rs->GetIntInRange(0, 9000) * percent + rs->GetIntInRange(0, 1000);
            if (random >= currentMax) {
                currentMax = random;
                action = pair.first;
            }
        }
        action.Apply(graph);
        actionMap.clear();
        iterations++;
    }

    int nodeCount = 0;
    for (string group : graph->GetGroups()) {
        nodeCount += graph->GetNodes(group).size();
    }
    Logger::Debug("Solved graph with " + to_string(graph->GetEdges().size()) + " edges / " + to_string(nodeCount) +
                  " nodes / " + to_string(graph->GetGroups().size()) + " groups in " + to_string(iterations) +
                  " iterations.");

    unordered_map<string, Node> results;
    for (auto node : graph->GetActiveNodes()) {
        results[node.GetGroup()] = node;
    }
    return results;
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

bool GraphAnalyzer::fillActionMap(const WeaverGraph &graph, map<GraphAction, float> *map,
                                  const AnalyzerParameters &param,
                                  shared_ptr<RandomStream> rs) {
    getSingleNodeActions(graph, map, param);
    getEdgeActions(graph, map, param, rs);
    return !map->empty();
}

float GraphAnalyzer::getGraphScore(const WeaverGraph &graph, const AnalyzerParameters &param) {
    float score = 0;
    auto mandatory = graph.GetMandatoryGroups();
    unordered_set<ID> activeNodeIds;

    // weigh the nodes
    for (auto node : graph.GetActiveNodes()) {
        activeNodeIds.insert(node.GetId());

        // penalty for optional nodes
        if (mandatory.count(node.GetGroup()) == 0) {
            score += param.optionalNodePenalty;
        }

        // penalty for new nodes
        if (node.GetId() == 0) {
            score += param.newNodePenalty;
        }

        // check for metaData activity on the node
        unordered_map<string, int> metaDataValues;
        for (auto metaData : node.GetHistory()) {
            for (string name : metaData.GetValueNames()) {
                auto iter = metaDataValues.find(name);
                int newValue = metaData.GetValue(name);
                if (iter == metaDataValues.end() || iter->second != newValue) {
                    score += param.MetaDataActivityBonus;
                }
            }
        }

        // check for common current metaData values
        score += abs(metaDataValues["relationToPlayer"]) / 100.0 * param.PlayerRelationshipBonus;
    }

    // weigh the edges
    for (auto edge : graph.GetEdges()) {
        if ((activeNodeIds.count(edge.GetNode1()) > 0) && (activeNodeIds.count(edge.GetNode2()) > 0)) {
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

void GraphAnalyzer::getSingleNodeActions(const WeaverGraph &graph, std::map<GraphAction, float> *map,
                                         const AnalyzerParameters &param) {
    float startScore = getGraphScore(graph, param);
    auto mandatoryGroups = graph.GetMandatoryGroups();
    for (string group : graph.GetGroups()) {
        bool isMandatory = mandatoryGroups.count(group) > 0;
        for (auto &node : graph.GetNodes(group)) {
            // this copy of the complete graph is not the best solution for performance,
            // but has better readability than mutating the graph all the time
            WeaverGraph graphCopy = graph;
            auto isActive = graph.IsNodeActive(node);

            GraphAction action(!isActive, node);
            if (isMandatory && isActive) {
                // we can not deactivate nodes in mandatory groups
                continue;
            }
            action.Apply(&graphCopy);
            float actionScore = getGraphScore(graphCopy, param);

            if (actionScore > startScore) {
                (*map)[action] = actionScore;
            }
        }
    }
}

void GraphAnalyzer::getEdgeActions(const WeaverGraph &graph, std::map<GraphAction, float> *map,
                                   const AnalyzerParameters &param, shared_ptr<RandomStream> rs) {
    float startScore = getGraphScore(graph, param);
    for (Edge edge : graph.GetEdges()) {
        auto nodes1 = graph.GetNodesWithId(edge.GetNode1());
        auto nodes2 = graph.GetNodesWithId(edge.GetNode2());
        if (nodes1.empty() || nodes2.empty()) {
            continue;
        }
        auto node1 = nodes1.at(rs->GetRandomIndex(nodes1.size()));
        auto node2 = nodes2.at(rs->GetRandomIndex(nodes2.size()));
        std::map<Node, bool> activeNodes;
        activeNodes[node1] = true;
        activeNodes[node2] = true;

        WeaverGraph graphCopy = graph;
        GraphAction action(activeNodes);
        action.Apply(&graphCopy);
        float actionScore = getGraphScore(graphCopy, param);

        if (actionScore > startScore) {
            (*map)[action] = actionScore;
        }
    }
}
