//
// Created by michael on 09.11.15.
//

#include <string>
#include <memory>
#include <Core/WeaverTypes.h>
#include <Core/Graph/GraphAnalyzer.h>
#include "catch.hpp"

using namespace weave;
using namespace std;

TEST_CASE("Graph analyzer", "[graph analyzer]") {
    shared_ptr<RandomStream> rs = make_shared<RandomStream>(42);
    WeaverGraph graph;

    SECTION("Graph action equality") {
        map<Node, bool> nodeActivations;
        Node node1("abc", 5);
        Node node2("abc", 5);
        Node node3("abX", 5);
        Node node4("abc", 6);

        SECTION("Equal empty actions") {
            GraphAction action1(nodeActivations);
            GraphAction action2(nodeActivations);
            REQUIRE(action1 == action2);
        }

        SECTION("Equal actions from map") {
            nodeActivations[node1] = true;
            nodeActivations[node4] = false;
            GraphAction action1(nodeActivations);
            GraphAction action2(nodeActivations);
            REQUIRE(action1 == action2);
        }

        SECTION("Equal actions from node") {
            GraphAction action1(true, node1);
            GraphAction action2(true, node2);
            REQUIRE(action1 == action2);
        }

        SECTION("Unequal actions from node") {
            GraphAction action1(true, node1);
            GraphAction action2(true, node3);
            GraphAction action3(true, node4);
            GraphAction action4(false, node3);
            GraphAction action5(false, node4);
            REQUIRE(!(action1 == action2));
            REQUIRE(!(action1 == action3));
            REQUIRE(!(action1 == action4));
            REQUIRE(!(action1 == action5));
        }
    }

    SECTION("Solve empty graph") {
        auto properties = GraphAnalyzer::SolveGraph(&graph, rs);
        REQUIRE(properties.size() == 0);
    }

    string groupA = "entityA";
    graph.CreateNodeGroup(groupA, true);
    Node node1(groupA, 1);
    graph.AddNode(node1);

    SECTION("Solve graph with single node") {
        graph.Finalize();
        auto properties = GraphAnalyzer::SolveGraph(&graph, rs);
        REQUIRE(properties.size() == 1);
        REQUIRE(properties[groupA] == node1);
        return;
    }

    Node node2(groupA, 2);
    graph.AddNode(node2);

    SECTION("Solve graph with two nodes / one group") {
        graph.Finalize();
        auto properties = GraphAnalyzer::SolveGraph(&graph, rs);
        REQUIRE(properties.size() == 1);
        REQUIRE(((properties[groupA] == node1) || (properties[groupA] == node2)));
        return;
    }

    string groupB = "entityB";
    graph.CreateNodeGroup(groupB, true);

    SECTION("Error on missing required node") {
        REQUIRE_THROWS_AS(graph.Finalize(), ContractFailedException);
        return;
    }

    Node node3(groupB, 3);
    graph.AddNode(node3);

    SECTION("Solve graph with three nodes / two groups") {
        graph.Finalize();
        auto properties = GraphAnalyzer::SolveGraph(&graph, rs);
        REQUIRE(properties.size() == 2);
        REQUIRE(((properties[groupA] == node1) || (properties[groupA] == node2)));
        REQUIRE((properties[groupB] == node3));
        return;
    }

    SECTION("Solve graph with three nodes / two groups / edge1-3") {
        Edge edge13(1, 3, EdgeType::DIRECT);
        graph.AddEdge(edge13);
        graph.Finalize();
        auto properties = GraphAnalyzer::SolveGraph(&graph, rs);
        REQUIRE(properties.size() == 2);
        REQUIRE((properties[groupA] == node1));
        REQUIRE((properties[groupB] == node3));
        return;
    }

    SECTION("Solve graph with three nodes / two groups / edge2-3") {
        Edge edge23(2, 3, EdgeType::DIRECT);
        graph.AddEdge(edge23);
        graph.Finalize();
        auto properties = GraphAnalyzer::SolveGraph(&graph, rs);
        REQUIRE(properties.size() == 2);
        REQUIRE((properties[groupA] == node2));
        REQUIRE((properties[groupB] == node3));
        return;
    }

    Edge edge13(1, 3, EdgeType::TRANSITIVE);
    string groupC = "entityC";
    Node node4(groupC, 4);
    graph.AddEdge(edge13).CreateNodeGroup(groupC, false).AddNode(node4);

    SECTION("Solve graph with four nodes / one optional") {
        graph.Finalize();
        auto properties = GraphAnalyzer::SolveGraph(&graph, rs);
        REQUIRE(properties.size() == 2);
        REQUIRE((properties[groupA] == node1));
        REQUIRE((properties[groupB] == node3));
        return;
    }

    Edge edge23(2, 3, EdgeType::DIRECT);
    graph.AddEdge(edge23);

    SECTION("Solve graph with two edges / direct edge takes precedence") {
        graph.Finalize();
        auto properties = GraphAnalyzer::SolveGraph(&graph, rs);
        REQUIRE(properties.size() == 2);
        REQUIRE((properties[groupA] == node2));
        REQUIRE((properties[groupB] == node3));
        return;
    }

    graph.AddShadowNode(5);

    SECTION("Error on double shadow node edge") {
        graph.AddShadowNode(42);
        Edge edge542(5, 42, EdgeType::DIRECT);
        REQUIRE_THROWS_AS(graph.AddEdge(edge542), ContractFailedException);
    }

    SECTION("Error on add node to finalized graph") {
        graph.Finalize();
        Node nodeX(groupA, 123);
        REQUIRE_THROWS_AS(graph.AddNode(nodeX), ContractFailedException);
        return;
    }

    SECTION("Error on add shadow node to finalized graph") {
        graph.Finalize();
        REQUIRE_THROWS_AS(graph.AddShadowNode(123), ContractFailedException);
        return;
    }

    SECTION("Error on add node group to finalized graph") {
        graph.Finalize();
        Node nodeX(groupA, 123);
        REQUIRE_THROWS_AS(graph.CreateNodeGroup("hi", false), ContractFailedException);
        return;
    }

    SECTION("Error on add edge to finalized graph") {
        graph.Finalize();
        Edge edge15(1, 5, EdgeType::DIRECT);
        REQUIRE_THROWS_AS(graph.AddEdge(edge15), ContractFailedException);
        return;
    }

    Edge edge15(1, 5, EdgeType::DIRECT);
    Edge edge25(5, 2, EdgeType::DIRECT);
    graph.AddEdge(edge15);
    graph.AddEdge(edge25);

    SECTION("Single transitive edge") {
        graph.Finalize();
        Edge searchEdge(1, 2, EdgeType::TRANSITIVE);
        auto edges = graph.GetEdges();
        auto iter = edges.find(searchEdge);
        REQUIRE(iter != edges.end());
        REQUIRE(iter->Count(EdgeType::TRANSITIVE) == 1);
        return;
    }

    graph.AddShadowNode(6);
    Edge edge16(6, 1, EdgeType::DIRECT);
    Edge edge26(2, 6, EdgeType::DIRECT);
    graph.AddEdge(edge16);
    graph.AddEdge(edge26);

    SECTION("Double transitive edge") {
        graph.Finalize();
        Edge searchEdge(1, 2, EdgeType::TRANSITIVE);
        auto edges = graph.GetEdges();
        auto iter = edges.find(searchEdge);
        REQUIRE(iter != edges.end());
        REQUIRE(iter->Count(EdgeType::TRANSITIVE) == 2);
        return;
    }
}
