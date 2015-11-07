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

    SECTION("Create edge between the same nodes") {
        REQUIRE_THROWS_AS(Edge(4, 4, EdgeType::DIRECT), ContractFailedException);
    }
}

TEST_CASE("API check", "[graph]") {
    SECTION("Node equality check") {
        Node emptyNode;
        Node node1("GroupX", 3);
        Node node2("GroupX", 3);
        Node node3("GroupY", 3);
        Node node4("GroupX", 4);

        REQUIRE(node1 == node1);
        REQUIRE(node1 == node2);
        REQUIRE(!(emptyNode == node1));
        REQUIRE(!(node1 == node3));
        REQUIRE(!(node1 == node4));
        REQUIRE(!(node3 == node4));
    }

    SECTION("Edge equality check") {
        Edge edge1(1, 2, EdgeType::DIRECT);
        Edge edge2(1, 2, EdgeType::TRANSITIVE);
        Edge edge3(2, 1, EdgeType::DIRECT);
        Edge edge4(1, 3, EdgeType::DIRECT);

        REQUIRE(edge1 == edge1);
        REQUIRE(edge1 == edge2);
        REQUIRE(edge1 == edge3);
        REQUIRE(!(edge1 == edge4));
        REQUIRE(!(edge2 == edge4));
        REQUIRE(!(edge3 == edge4));
    }
}
