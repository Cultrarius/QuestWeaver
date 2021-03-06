//
// Created by michael on 12.10.15.
//

#include <string>
#include <Core/Graph/WeaverGraph.h>
#include "catch.hpp"

using namespace weave;
using namespace std;

TEST_CASE("Graph failures", "[graph]") {
    WeaverGraph graph;

    SECTION("Create identical node group") {
        graph.CreateNodeGroup("TestGroup", false);
        REQUIRE_THROWS_AS(graph.CreateNodeGroup("TestGroup", false), ContractFailedException);
    }

    SECTION("Create identical node") {
        graph.CreateNodeGroup("TestGroup", false).AddNode(Node("TestGroup", 7));
        REQUIRE_THROWS_AS(graph.AddNode(Node("TestGroup", 7)), ContractFailedException);
    }

    SECTION("Get from unknown group") {
        REQUIRE_THROWS_AS(graph.GetNodes("Unknown"), ContractFailedException);
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
        graph.CreateNodeGroup("TestGroup", false).AddNode(Node("TestGroup", 7));
        REQUIRE_THROWS_AS(graph.AddEdge(Edge(4, 7, EdgeType::DIRECT)), ContractFailedException);
    }

    SECTION("Create edge between the same nodes") {
        REQUIRE_THROWS_AS(Edge(4, 4, EdgeType::DIRECT), ContractFailedException);
    }

    SECTION("Activate unknown node") {
        REQUIRE_THROWS_AS(graph.ActivateNode(Node("Test", 2)), ContractFailedException);
    }

    SECTION("Deactivate unknown node") {
        REQUIRE_THROWS_AS(graph.DeactivateNode(Node("Test", 2)), ContractFailedException);
    }

    SECTION("Activate node from unknown group") {
        graph.CreateNodeGroup("Test123", false).AddNode(Node("Test123", 2));
        REQUIRE_THROWS_AS(graph.ActivateNode(Node("Test", 2)), ContractFailedException);
    }

    SECTION("Deactivate node from unknown group") {
        graph.CreateNodeGroup("Test123", false).AddNode(Node("Test123", 2));
        REQUIRE_THROWS_AS(graph.DeactivateNode(Node("Test", 2)), ContractFailedException);
    }

    SECTION("Double shadow node") {
        graph.AddShadowNode(4);
        REQUIRE_THROWS_AS(graph.AddShadowNode(4), ContractFailedException);
    }

    SECTION("Shadow node for regular node") {
        graph.CreateNodeGroup("asd", false).AddNode(Node("asd", 4));
        REQUIRE_THROWS_AS(graph.AddShadowNode(4), ContractFailedException);
    }
}

TEST_CASE("API check", "[graph]") {
    SECTION("Node equality") {
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

    SECTION("Edge equality") {
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

    SECTION("Edge id getter") {
        Edge edge1(1, 2, EdgeType::DIRECT);
        Edge edge2(2, 1, EdgeType::DIRECT);

        REQUIRE(edge1.Get(1) == 2);
        REQUIRE(edge1.Get(2) == 1);
        REQUIRE(edge2.Get(1) == 2);
        REQUIRE(edge2.Get(2) == 1);
    }

    SECTION("Edge id getter") {
        Edge edge1(1, 2, EdgeType::DIRECT);
        Edge edge2(2, 1, EdgeType::DIRECT);

        REQUIRE(edge1.Get(1) == 2);
        REQUIRE(edge1.Get(2) == 1);
        REQUIRE(edge2.Get(1) == 2);
        REQUIRE(edge2.Get(2) == 1);
    }

    SECTION("Edge type count") {
        Edge edge1(1, 2, EdgeType::DIRECT);
        REQUIRE(edge1.Count(EdgeType::DIRECT) == 1);
        REQUIRE(edge1.Count(EdgeType::TRANSITIVE) == 0);
        REQUIRE(edge1.GetQuestIds().size() == 0);
    }

    WeaverGraph graph;
    string group1 = "TestGroup";
    string group2 = "TestGroup2";
    string group3 = "TestGroup3";
    Edge edge1(1, 2, EdgeType::DIRECT, 9);
    Node node1 = Node(group1, 1);
    Node node2 = Node(group2, 2);
    graph.CreateNodeGroup(group1, false)
            .CreateNodeGroup(group2, true)
            .AddNode(node1)
            .AddNode(node2)
            .AddEdge(edge1);

    SECTION("Graph edge getter one edge") {
        REQUIRE(graph.GetEdges().size() == 1);
        REQUIRE(*graph.GetEdges().begin() == edge1);
    }

    SECTION("Graph edge getter double edge") {
        Edge edge2(2, 1, EdgeType::DIRECT, 10);
        graph.AddEdge(edge2);

        REQUIRE(graph.GetEdges().size() == 1);
        REQUIRE(graph.GetEdges().begin()->Count(EdgeType::DIRECT) == 2);
        REQUIRE(graph.GetEdges().begin()->Count(EdgeType::TRANSITIVE) == 0);
        REQUIRE(graph.GetEdges().begin()->GetQuestIds().size() == 2);
    }

    SECTION("Graph edge getter two edges") {
        Node node3 = Node(group3, 3);
        Edge edge2(2, 3, EdgeType::DIRECT, 10);
        graph.CreateNodeGroup(group3, true).AddNode(node3).AddEdge(edge2);

        REQUIRE(graph.GetEdges().size() == 2);
        REQUIRE(graph.GetEdges().begin()->GetQuestIds().size() == 1);
    }

    SECTION("Graph discard edge between nodes of same group") {
        Node node3 = Node(group2, 3);
        Edge edge2(2, 3, EdgeType::TRANSITIVE);
        graph.CreateNodeGroup(group3, true).AddNode(node3).AddEdge(edge2);

        REQUIRE(graph.GetEdges().size() == 1);
        REQUIRE(graph.GetEdges().begin()->Count(EdgeType::DIRECT) == 1);
        REQUIRE(graph.GetEdges().begin()->Count(EdgeType::TRANSITIVE) == 0);
    }
}

TEST_CASE("Node activation", "[graph]") {
    WeaverGraph graph;

    SECTION("Get empty node list") {
        REQUIRE(graph.GetNodesWithId(42).empty());
    }

    string group1 = "TestGroup";
    string group2 = "TestGroup2";
    Node node1 = Node(group1, 1);
    Node node2 = Node(group2, 2);
    graph.CreateNodeGroup(group1, false)
            .CreateNodeGroup(group2, true)
            .AddNode(node1)
            .AddNode(node2);

    SECTION("Activate node") {
        REQUIRE(graph.GetActiveNodes().empty());
        graph.ActivateNode(node1);
        REQUIRE(graph.GetActiveNodes().size() == 1);
    }

    SECTION("Deactivate optional node") {
        REQUIRE(graph.GetActiveNodes().empty());
        graph.ActivateNode(node1);
        REQUIRE(graph.DeactivateNode(node1));
        REQUIRE(graph.GetActiveNodes().empty());
    }

    SECTION("Deactivate mandatory node") {
        REQUIRE(graph.GetActiveNodes().empty());
        graph.ActivateNode(node2);
        REQUIRE_FALSE(graph.DeactivateNode(node2));
        REQUIRE(graph.GetActiveNodes().size() == 1);
    }
}