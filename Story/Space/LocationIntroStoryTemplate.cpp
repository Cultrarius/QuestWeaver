//
// Created by michael on 12.03.16.
//

#include <World/Space/SolarSystem.h>
#include "Story/Space/LocationIntroStoryTemplate.h"

using namespace weave;
using namespace std;

StoryTemplateResult LocationIntroStoryTemplate::CreateStory(const EntityMap &requiredEntities, const WeaverGraph &,
                                                            const WorldModel &worldModel) const {
    auto entities = getValidEntities(requiredEntities, worldModel);
    if (entities.empty()) {
        throw ContractFailedException("Invalid template call!");
    }
    StoryTemplateResult result;
    auto entity = entities[0];

    TokenToEntityMap tokenEntityMap;
    tokenEntityMap["1"] = {entity->GetId()};
    result.tokenMap = createTokenMapping(tokenEntityMap);
    result.rawText = rawText;

    MetaData markedData;
    markedData.SetValue(metaDataMarker, 1);
    result.worldActions.emplace_back(WorldActionType::UPDATE, entity, markedData);

    return result;
}

bool LocationIntroStoryTemplate::IsValid(const EntityMap &requiredEntities, const WeaverGraph &,
                                         const WorldModel &worldModel) const {
    return getValidEntities(requiredEntities, worldModel).size() > 0;
}

vector<shared_ptr<WorldEntity>> LocationIntroStoryTemplate::getValidEntities(const EntityMap &entityMap,
                                                                             const WorldModel &worldModel) const {
    vector<shared_ptr<WorldEntity>> result;
    auto mapIter = entityMap.find(SolarSystem::Type);
    if (mapIter == entityMap.end()) {
        return result;
    }
    for (shared_ptr<WorldEntity> entity : mapIter->second) {
        ID id = entity->GetId();
        MetaData metaData = worldModel.GetMetaData(id);
        if (metaData.HasValue(metaDataMarker)) {
            continue;
        }
        result.push_back(entity);
    }
    return result;
}
