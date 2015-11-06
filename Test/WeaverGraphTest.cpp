//
// Created by michael on 12.10.15.
//

#include <string>
#include "catch.hpp"
#include "../Core/WeaverUtils.h"
#include "../Core/Graph/WeaverGraph.h"

using namespace weave;
using namespace std;

TEST_CASE("Graph failures", "[graph]") {
    WeaverGraph graph;

    SECTION("Create identical node group") {
        graph.CreateNodeGroup("TestGroup", false);
        REQUIRE_THROWS_AS(graph.CreateNodeGroup("TestGroup", false), ContractFailedException);
    }

    SECTION("Add node with empty group") {
        REQUIRE_THROWS_AS(graph.AddNode(Node()), ContractFailedException);
    }

    SECTION("Add node with unknown group") {
        REQUIRE_THROWS_AS(graph.AddNode(Node("TestGroup", 7)), ContractFailedException);
    }

    SECTION("Add edge with unknown nodes") {
        REQUIRE_THROWS_AS(graph.AddEdge(Edge(4, 7, EdgeType::DIRECT)), ContractFailedException);
    }

    SECTION("Add edge with unknown nodes") {
        graph.CreateNodeGroup("TestGroup", false);
        graph.AddNode(Node("TestGroup", 7));
        REQUIRE_THROWS_AS(graph.AddEdge(Edge(4, 7, EdgeType::DIRECT)), ContractFailedException);
    }
}

TEST_CASE("API check", "[graph]") {
    SECTION("Node similarity check") {
        Node emptyNode;
        Node node1("GroupX", 3);
        Node node2("GroupX", 3);
        Node node3("GroupY", 3);
        Node node4("GroupX", 4);

        REQUIRE(node1 == node2);
        REQUIRE(!(emptyNode == node1));
        REQUIRE(!(node1 == node3));
        REQUIRE(!(node1 == node4));
        REQUIRE(!(node3 == node4));
    }
}
