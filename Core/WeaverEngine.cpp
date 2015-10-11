//
// Created by michael on 10.08.15.
//

#include "WeaverEngine.h"
#include <iostream>

using namespace std;
using namespace weave;

vector<QuestPropertyValue> WeaverEngine::fillTemplate(shared_ptr<Template> questTemplate,
                                                      const WorldModel &worldModel,
                                                      std::shared_ptr<RandomStream> randomStream) const {
    // TODO insert fancy algorithm
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
        const vector<ModelAction> candidates = questTemplate->GetPropertyCandidates(property, worldModel, randomStream);

        shared_ptr<WorldEntity> entity = candidates[0].GetEntity();
        QuestPropertyValue questValue(property, entity);
    }

    return returnValues;
}
