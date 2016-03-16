//
// Created by michael on 06.10.15.
//

#include <World/Space/SpaceWorldModel.h>

using namespace std;
using namespace weave;

WorldModelAction SpaceWorldModel::CreateLocation() const {
    double x = rs->GetIntInRange(param.minLocation, param.maxLocation);
    double y = rs->GetIntInRange(param.minLocation, param.maxLocation);
    double z = rs->GetIntInRange(param.minLocation, param.maxLocation);
    return WorldModelAction(WorldActionType::CREATE, make_shared<SpaceLocation>(x, y, z));
}

SpaceWorldModel::SpaceWorldModel(std::shared_ptr<RandomStream> randomStream) : WorldModel() {
    rs = randomStream;
}

ModelParameters SpaceWorldModel::GetParameters() {
    return param;
}

void SpaceWorldModel::SetParameters(ModelParameters parameters) {
    this->param = parameters;
}

const NameGenerator &SpaceWorldModel::GetNameGenerator() const {
    return nameGenerator;
}

WorldModelAction SpaceWorldModel::CreateAgent(NameType nameType) const {
    return WorldModelAction(WorldActionType::CREATE, make_shared<SpaceAgent>(nameGenerator.CreateName(nameType, rs)));
}

vector<WorldModelAction> SpaceWorldModel::CreateSolarSystem(NameType nameType) const {
    vector<WorldModelAction> actions;

    auto locationAction = CreateLocation();
    actions.push_back(locationAction);
    shared_ptr<SpaceLocation> location = dynamic_pointer_cast<SpaceLocation>(locationAction.GetEntity());
    auto solarSystem = make_shared<SolarSystem>(nameGenerator.CreateName(nameType, rs), location);
    actions.push_back(WorldModelAction(WorldActionType::CREATE, solarSystem));

    return actions;
}
