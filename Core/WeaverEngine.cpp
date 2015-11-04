//
// Created by michael on 10.08.15.
//

#include "WeaverEngine.h"
#include "Graph/WeaverGraph.h"
#include <iostream>

using namespace std;
using namespace weave;

vector<QuestPropertyValue> WeaverEngine::fillTemplate(shared_ptr<Template> questTemplate,
                                                      const WorldModel &worldModel,
                                                      std::shared_ptr<RandomStream> randomStream,
                                                      std::vector<ModelAction> *modelActions) const {
    if (parameters.useDice) {
        return fillWithRandomDice(questTemplate, worldModel, randomStream, modelActions);
    } else {
        vector<QuestPropertyValue> returnValues;
        unordered_set<string> mandatory;
        map<string, vector<ModelAction>> candidates;
        for (const TemplateQuestProperty &questProperty : questTemplate->GetProperties()) {
            candidates[questProperty.GetName()] = questTemplate->GetPropertyCandidates(questProperty, worldModel);
            if (questProperty.IsMandatory()) {
                mandatory.insert(questProperty.GetName());
            }
        }

        WeaverGraph graph;
        for (auto pair : candidates) {
            auto groupName = pair.first;
            bool isMandatory = mandatory.find(groupName) != mandatory.end();
            graph.createNodeGroup(groupName, isMandatory);
            for (auto candidate : pair.second) {
                graph.addNode(Node(groupName, candidate.GetEntity()->GetId()));
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

vector<QuestPropertyValue> WeaverEngine::fillWithRandomDice(const shared_ptr<Template> &questTemplate,
                                                            const WorldModel &worldModel,
                                                            shared_ptr<RandomStream> randomStream,
                                                            vector<ModelAction> *modelActions) const {
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
        const vector<ModelAction> candidates = questTemplate->GetPropertyCandidates(property, worldModel);
        int index = randomStream->GetRandomIndex(candidates.size());
        shared_ptr<WorldEntity> entity = candidates[index].GetEntity();
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
