//
// Created by michael on 09.11.15.
//

#include <string>
#include <memory>
#include "catch.hpp"
#include "../Core/WeaverUtils.h"
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

    Node node2(groupA, 2);
    graph.AddNode(node2);

    SECTION("Solve graph with two nodes") {
        auto properties = GraphAnalyzer::SolveGraph(&graph, rs);
        REQUIRE(properties.size() == 1);
        REQUIRE(((properties[groupA] == node1) || (properties[groupA] == node2)));
    }

    string groupB = "entityB";
    graph.CreateNodeGroup(groupB, true);

    SECTION("Error on missing required node") {
        REQUIRE_THROWS_AS(GraphAnalyzer::SolveGraph(&graph, rs), ContractFailedException);
    }
}
