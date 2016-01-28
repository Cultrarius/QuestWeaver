//
// Created by michael on 10.08.15.
//

#include <iostream>
#include <Core/WeaverEngine.h>

using namespace std;
using namespace weave;

EngineResult WeaverEngine::fillTemplate(shared_ptr<Template> questTemplate,
                                        const QuestModel &questModel,
                                        const WorldModel &worldModel,
                                        const StoryWriter &storyWriter) const {
    unordered_set<string> mandatory;
    map<string, vector<WorldModelAction>> candidates;
    for (const TemplateQuestProperty &questProperty : questTemplate->GetProperties()) {
        candidates[questProperty.GetName()] = questTemplate->GetPropertyCandidates(questProperty, worldModel);
        if (questProperty.IsMandatory()) {
            mandatory.insert(questProperty.GetName());
        }
    }

    WeaverGraph graph = createGraph(questModel, worldModel, mandatory, candidates);
    auto propertyMap = GraphAnalyzer::SolveGraph(&graph, randomStream);

    vector<QuestPropertyValue> propertyValues;
    vector<WorldModelAction> modelActions;
    for (auto &questProperty : questTemplate->GetProperties()) {
        const string &propertyName = questProperty.GetName();
        auto iter = propertyMap.find(propertyName);
        if (iter == propertyMap.end()) {
            if (questProperty.IsMandatory()) {
                throw ContractFailedException("Graph result must contain results for mandatory quest properties.");
            }
            continue;
        }
        ID nodeId = iter->second.GetId();
        for (auto candidate : candidates[propertyName]) {
            if (candidate.GetEntity()->GetId() == nodeId) {
                modelActions.push_back(candidate);
                QuestPropertyValue questValue(questProperty, candidate.GetEntity());
                propertyValues.push_back(move(questValue));
            }
        }
    }
    string story = storyWriter.CreateStory(graph);
    return EngineResult(modelActions, propertyValues, story);
}

WeaverGraph WeaverEngine::createGraph(const QuestModel &questModel, const WorldModel &worldModel,
                                      unordered_set<string> mandatory,
                                      map<string, vector<WorldModelAction>> candidates) const {
    WeaverGraph graph;
    unordered_set<ID> candidateIds;
    for (auto pair : candidates) {
        auto groupName = pair.first;
        bool isMandatory = mandatory.find(groupName) != mandatory.end();
        graph.CreateNodeGroup(groupName, isMandatory);
        for (auto candidate : pair.second) {
            ID id = candidate.GetEntity()->GetId();

            // add metadata for node
            vector<MetaData> metaData;
            for (auto action : worldModel.GetMetaDataHistoryForId(id)) {
                metaData.push_back(action.GetMetaData());
            }

            graph.AddNode(Node(groupName, id, metaData));
            candidateIds.insert(id);
        }
    }

    addGraphEdges(questModel, graph, candidateIds);
    graph.Finalize();
    return graph;
}

void WeaverEngine::addGraphEdges(const QuestModel &questModel, WeaverGraph &graph,
                                 unordered_set<ID> &candidateIds) const {
    for (auto quest : questModel.GetQuests()) {
        ID questId = quest->GetId();
        auto questEntities = questModel.GetQuestEntities(questId);

        unordered_set<ID> entityIds;
        unordered_set<ID> shadowIds;
        for (auto entity : questEntities) {
            ID id = entity->GetId();
            // add unknown entities as shadow node, the graph will use them
            // to create the transitive edges between the nodes
            if (candidateIds.find(id) == candidateIds.end()) {
                if (shadowIds.find(id) == shadowIds.end()) {
                    graph.AddShadowNode(id);
                    shadowIds.insert(id);
                }
                continue;
            }
            entityIds.insert(id);
        }

        // add direct connection history
        unordered_set<ID> seenIds;
        for (ID id1 : entityIds) {
            seenIds.insert(id1);
            for (ID id2 : entityIds) {
                if (seenIds.find(id2) != seenIds.end()) {
                    continue;
                }
                Edge edge(id1, id2, EdgeType::DIRECT, questId);
                graph.AddEdge(edge);
            }
            for (ID id2 : shadowIds) {
                Edge edge(id1, id2, EdgeType::DIRECT, questId);
                graph.AddEdge(edge);
            }
        }
    }
}

EngineParameters WeaverEngine::GetParameters() {
    return parameters;
}

void WeaverEngine::SetParameters(EngineParameters params) {
    this->parameters = params;
}

WeaverEngine::WeaverEngine(std::shared_ptr<RandomStream> rs) {
    this->randomStream = rs;
}

EngineResult::EngineResult(const std::vector<WorldModelAction> &actions,
                           const std::vector<QuestPropertyValue> &propertyValues,
                           const std::string &story) : actions(actions), propertyValues(propertyValues), story(story) {
}

const std::vector<WorldModelAction> &EngineResult::GetModelActions() const {
    return actions;
}

const std::vector<QuestPropertyValue> &EngineResult::GetQuestPropertyValues() const {
    return propertyValues;
}

const std::string &EngineResult::GetStory() const {
    return story;
}
