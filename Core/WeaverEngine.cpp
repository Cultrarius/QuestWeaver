//
// Created by michael on 10.08.15.
//

#include <iostream>
#include <iterator>
#include <Core/WeaverEngine.h>

using namespace std;
using namespace weave;

EngineResult WeaverEngine::fillTemplate(shared_ptr<QuestTemplate> questTemplate,
                                        const QuestModel &questModel,
                                        const WorldModel &worldModel) const {
    unordered_set<string> mandatory;
    map<string, vector<PropertyCandidate>> candidates;
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
                string errorMsg = "Graph result must contain results for mandatory quest properties.";
                Logger::Fatal(ContractFailedException(errorMsg));
            }
            continue;
        }
        ID nodeId = iter->second.GetId();
        for (auto candidate : candidates[propertyName]) {
            if (candidate.GetValue()->GetId() != nodeId) {
                continue;
            }
            for (auto action : candidate.GetActions()) {
                modelActions.push_back(action);
            }
            propertyValues.emplace_back(questProperty, candidate.GetValue());
        }
    }
    StoryWriterParameters storyParams(graph, propertyValues, modelActions);
    return EngineResult(modelActions, propertyValues, storyParams);
}

WeaverGraph WeaverEngine::createGraph(const QuestModel &questModel, const WorldModel &worldModel,
                                      unordered_set<string> mandatory,
                                      map<string, vector<PropertyCandidate>> candidates) const {
    WeaverGraph graph;
    unordered_set<ID> candidateIds;
    for (auto pair : candidates) {
        auto groupName = pair.first;
        bool isMandatory = mandatory.count(groupName) > 0;
        graph.CreateNodeGroup(groupName, isMandatory);
        unordered_set<ID> seenIds;
        for (auto candidate : pair.second) {
            ID id = candidate.GetValue()->GetId();
            if (seenIds.count(id) > 0) {
                continue;
            }
            seenIds.insert(id);

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
            if (candidateIds.count(id) == 0) {
                if (shadowIds.count(id) == 0) {
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
                if (seenIds.count(id2) > 0) {
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
                           const StoryWriterParameters &storyParams) : actions(actions), propertyValues(propertyValues),
                                                                       storyParams(storyParams) {
}

const std::vector<WorldModelAction> &EngineResult::GetModelActions() const {
    return actions;
}

const std::vector<QuestPropertyValue> &EngineResult::GetQuestPropertyValues() const {
    return propertyValues;
}

const StoryWriterParameters &EngineResult::GetStoryParameters() const {
    return storyParams;
}
