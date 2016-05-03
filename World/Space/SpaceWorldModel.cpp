//
// Created by michael on 06.10.15.
//

#define _USE_MATH_DEFINES

#include <cmath>
#include <World/Space/SpaceWorldModel.h>
#include <World/Space/SpaceLocation.h>
#include <World/Space/SpaceAgent.h>
#include <World/Space/SolarSystem.h>
#include <World/Space/DeadCivilization.h>
#include <World/Space/Artifact.h>

using namespace std;
using namespace weave;

WorldModelAction SpaceWorldModel::CreateLocation() const {
    int x = rs->GetNormalIntInRange(-param.maxLocation, param.maxLocation);
    int y = rs->GetNormalIntInRange(-param.maxLocation, param.maxLocation);
    int z = rs->GetNormalIntInRange(-param.maxLocation, param.maxLocation);
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

WorldModelAction SpaceWorldModel::CreatePlanet(shared_ptr<SpaceLocation> location, NameType nameType,
                                               int distanceToSun) const {
    float angle = rs->GetULongInRange(0, 3600) / 10.0f;
    float radians = static_cast<float>(angle * M_PI / 180);
    float x = distanceToSun * cos(radians);
    float y = distanceToSun * sin(radians);
    int seed = rs->GetInt();
    string name = nameGenerator.CreateName(nameType, rs);
    MetaData metaData;
    if (rs->GetIntInRange(0, param.deadCivRarity) == param.deadCivRarity) {
        metaData.SetValue(DeadCivilization::PlanetMarker, 1);
    }
    return WorldModelAction(WorldActionType::CREATE, make_shared<Planet>(x, y, seed, name, location), metaData);
}

vector<WorldModelAction> SpaceWorldModel::CreateSolarSystem(NameType nameType, int planetCount) const {
    if (planetCount < 0) {
        planetCount = rs->GetNormalIntInRange(param.minPlanets, param.maxPlanets);
    }

    vector<WorldModelAction> actions;

    auto locationAction = CreateLocation();
    actions.push_back(locationAction);
    shared_ptr<SpaceLocation> location = dynamic_pointer_cast<SpaceLocation>(locationAction.GetEntity());

    vector<int> availableRings;
    for (int i = 1; i <= param.maxPlanets; i++) {
        availableRings.push_back(i);
    }
    vector<NameType> planetNames = {NameType::DARK_THING, NameType::ALIEN, NameType::LIGHT_THING};
    vector<shared_ptr<Planet>> planets;
    for (int i = 1; i <= planetCount; i++) {
        uint64_t ringIndex = rs->GetRandomIndex(availableRings.size());
        int ring = availableRings[ringIndex];
        availableRings.erase(availableRings.begin() + ringIndex);
        int var = param.planetDistanceVariation;
        int planetDistanceVariation = rs->GetNormalIntInRange(-var, var);
        int distance = param.planetDistanceBase + planetDistanceVariation + ring * param.planetDistanceAverage;
        auto nameType = planetNames[rs->GetRandomIndex(planetNames.size())];
        auto planetAction = CreatePlanet(location, nameType, distance);
        actions.push_back(planetAction);
        planets.push_back(dynamic_pointer_cast<Planet>(planetAction.GetEntity()));
    }

    int seed = rs->GetInt();
    auto solarSystem = make_shared<SolarSystem>(nameGenerator.CreateName(nameType, rs), seed, location, planets);
    actions.push_back(WorldModelAction(WorldActionType::CREATE, solarSystem));

    return actions;
}

WorldModelAction SpaceWorldModel::CreateDeadCivilization(NameType nameType) const {
    string name = nameGenerator.CreateName(nameType, rs);
    return WorldModelAction(WorldActionType::CREATE, make_shared<DeadCivilization>(name));
}

WorldModelAction SpaceWorldModel::CreateArtifact(NameType nameType) const {
    string name = nameGenerator.CreateName(nameType, rs);
    return WorldModelAction(WorldActionType::CREATE, make_shared<Artifact>(name));
}


