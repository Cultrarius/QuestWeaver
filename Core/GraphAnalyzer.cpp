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
        graph->activateNode(nodes[index]);
    }


    int score = 0;
    unordered_map<string, Node> results;
    return results;
}
