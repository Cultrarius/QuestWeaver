//
// Created by michael on 06.10.15.
//

#include <World/Space/SpaceWorldModel.h>
#include <World/Space/SpaceLocation.h>
#include <World/Space/SpaceAgent.h>
#include <World/Space/SolarSystem.h>
#include <World/Space/DeadCivilization.h>

using namespace std;
using namespace weave;

WorldModelAction SpaceWorldModel::CreateLocation() const {
    int x = rs->GetIntInRange(param.minLocation, param.maxLocation);
    int y = rs->GetIntInRange(param.minLocation, param.maxLocation);
    int z = rs->GetIntInRange(param.minLocation, param.maxLocation);
    return WorldModelAction(WorldActionType::CREATE, make_shared<SpaceLocation>(x, y, z));
}

SpaceWorldModel::SpaceWorldModel(std::shared_ptr<RandomStream> randomStream) : WorldModel() {
    rs = randomStream;
}

SpaceWorldModel::SpaceWorldModel(std::shared_ptr<RandomStream> randomStream, ModelParameters modelParameters) :
        WorldModel(), param(modelParameters) {
    rs = randomStream;
}

ModelParameters SpaceWorldModel::GetParameters() const {
    return param;
}

const NameGenerator &SpaceWorldModel::GetNameGenerator() const {
    return nameGenerator;
}

WorldModelAction SpaceWorldModel::CreateAgent(NameType nameType) const {
    return WorldModelAction(WorldActionType::CREATE, make_shared<SpaceAgent>(nameGenerator.CreateName(nameType, rs)));
}

WorldModelAction SpaceWorldModel::CreatePlanet(NameType nameType, int distanceToSun) const {
    float angle = rs->GetULongInRange(0, 3600) / 10.0f;
    float radians = static_cast<float>(angle * M_PI / 180);
    float x = distanceToSun * cos(radians);
    float y = distanceToSun * sin(radians);
    int seed = rs->GetInt();
    string name = nameGenerator.CreateName(nameType, rs);
    return WorldModelAction(WorldActionType::CREATE, make_shared<Planet>(x, y, seed, name));
}

vector<WorldModelAction> SpaceWorldModel::CreateSolarSystem(NameType nameType, int planetCount) const {
    if (planetCount < 0) {
        planetCount = rs->GetNormalIntInRange(param.minPlanets, param.maxPlanets);
    }

    vector<WorldModelAction> actions;
    vector<NameType> planetNames = {NameType::DARK_THING, NameType::ALIEN, NameType::LIGHT_THING};
    vector<shared_ptr<Planet>> planets;
    for (int i = 1; i <= planetCount; i++) {
        int var = param.planetDistanceVariation;
        int distance = param.planetDistanceBase + rs->GetNormalIntInRange(-var, var);
        auto nameType = planetNames[rs->GetRandomIndex(planetNames.size())];
        auto planetAction = CreatePlanet(nameType, distance);
        actions.push_back(planetAction);
        planets.push_back(dynamic_pointer_cast<Planet>(planetAction.GetEntity()));
    }

    auto locationAction = CreateLocation();
    actions.push_back(locationAction);
    shared_ptr<SpaceLocation> location = dynamic_pointer_cast<SpaceLocation>(locationAction.GetEntity());
    auto solarSystem = make_shared<SolarSystem>(nameGenerator.CreateName(nameType, rs), location, planets);
    actions.push_back(WorldModelAction(WorldActionType::CREATE, solarSystem));

    return actions;
}

WorldModelAction SpaceWorldModel::CreateDeadCivilization(NameType nameType) const {
    string name = nameGenerator.CreateName(nameType, rs);
    return WorldModelAction(WorldActionType::CREATE, make_shared<DeadCivilization>(name));
}