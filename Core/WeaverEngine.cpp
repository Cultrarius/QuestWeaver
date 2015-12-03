//
// Created by michael on 10.08.15.
//

#include <iostream>
#include "WeaverEngine.h"

using namespace std;
using namespace weave;

vector<QuestPropertyValue> WeaverEngine::fillTemplate(shared_ptr<Template> questTemplate,
                                                      const QuestModel &questModel,
                                                      const WorldModel &worldModel,
                                                      shared_ptr<RandomStream> randomStream,
                                                      vector<WorldModelAction> *modelActions) const {
    if (parameters.useDice) {
        return fillWithRandomDice(questTemplate, worldModel, randomStream, modelActions);
    } else {
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

        vector<QuestPropertyValue> returnValues;
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
                    if (modelActions != nullptr) {
                        modelActions->push_back(candidate);
                    }
                    QuestPropertyValue questValue(questProperty, candidate.GetEntity());
                    returnValues.push_back(move(questValue));
                }
            }
        }

        // build "graph" of candidates
        // each edge (+weight) is determined by the following:
        //   * direct quest history (both candidate entities were present in a past quest)
        //   * indirect quest history (first grade transitive relation)
        //   * candidates competing with each other have no edges (duh!)
        // each node weight is determined by the following:
        //   * metadata history of the entity (a lot of history = better)
        //   * specific current metadata? e.g. relationship to player
        //   * quest history of the entity
        // edges might have their own type and weight depending on the metadata and the action
        // search for a node/edge configuration that has a high weight
        //   * activating one node deactivates all other nodes competing for the same property
        //
        // optional: create some kind of "fluff" from the decision to add to the quest description
        // e.g. "Ever since the incident on planet Aurelius, your reputation with the Xerxes group
        // has been steadily declining. Now they are out to get you..."


        return returnValues;
    }
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
        auto questEntities = questModel.GetQuestEntities(quest->GetId());

        // add direct connection history
        unordered_set<ID> entityIds;
        unordered_set<ID> shadowIds;
        for (auto entity : questEntities) {
            ID id = entity->GetId();
            if (candidateIds.find(id) == candidateIds.end()) {
                if (shadowIds.find(id) == shadowIds.end()) {
                    graph.AddShadowNode(id);
                    shadowIds.insert(id);
                }
                continue;
            }
            entityIds.insert(id);
        }
        unordered_set<ID> seenIds;
        for (ID id1 : entityIds) {
            seenIds.insert(id1);
            for (ID id2 : entityIds) {
                if (seenIds.find(id2) != seenIds.end()) {
                    continue;
                }
                Edge edge(id1, id2, EdgeType::DIRECT);
                graph.AddEdge(edge);
            }
            for (ID id2 : shadowIds) {
                Edge edge(id1, id2, EdgeType::DIRECT);
                graph.AddEdge(edge);
            }
        }
    }
}

vector<QuestPropertyValue> WeaverEngine::fillWithRandomDice(const shared_ptr<Template> &questTemplate,
                                                            const WorldModel &worldModel,
                                                            shared_ptr<RandomStream> randomStream,
                                                            vector<WorldModelAction> *modelActions) const {
    vector<QuestPropertyValue> returnValues;
    vector<TemplateQuestProperty> propertiesToCreate;
    for (const TemplateQuestProperty &questProperty : questTemplate->GetProperties()) {
        if (questProperty.IsMandatory() || randomStream->GetIntInRange(0, 100) < 50) {
            propertiesToCreate.push_back(questProperty);
        } else {
            cout << "Omitting property " << questProperty.GetName() << endl;
        }
    }

    for (const auto &property : propertiesToCreate) {
        const vector<WorldModelAction> candidates = questTemplate->GetPropertyCandidates(property, worldModel);
        auto index = randomStream->GetRandomIndex(candidates.size());
        shared_ptr<WorldEntity> entity = candidates.at(index).GetEntity();
        if (modelActions != nullptr) {
            modelActions->push_back(candidates[index]);
        }
        QuestPropertyValue questValue(property, entity);
        returnValues.push_back(move(questValue));
    }

    return returnValues;
}

EngineParameters WeaverEngine::GetParameters() {
    return parameters;
}

void WeaverEngine::SetParameters(EngineParameters parameters) {
    this->parameters = parameters;
}
