//
// Created by michael on 09.11.15.
//

#include <string>
#include <memory>
#include "catch.hpp"
#include "../Core/WeaverUtils.h"
#include "../Template/Space/SpaceQuestTemplateFactory.h"
#include "../World/Space/SpaceWorldModel.h"
#include "../Core/GraphAnalyzer.h"

using namespace weave;
using namespace std;

TEST_CASE("Graph analyzer", "[graph analyzer]") {
    shared_ptr<RandomStream> rs = make_shared<RandomStream>(42);
    WeaverGraph graph;

    SECTION("Solve empty graph") {
        auto properties = GraphAnalyzer::SolveGraph(&graph, rs);
        REQUIRE(properties.size() == 0);
    }

    string groupA = "entityA";
    graph.CreateNodeGroup(groupA, true);
    Node node1(groupA, 1);
    graph.AddNode(node1);

    SECTION("Solve graph with single node") {
        auto properties = GraphAnalyzer::SolveGraph(&graph, rs);
        REQUIRE(properties.size() == 1);
        REQUIRE(properties[groupA] == node1);
    }
}
