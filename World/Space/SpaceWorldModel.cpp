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
#include <World/Space/SpaceStation.h>

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
    actions.emplace_back(WorldActionType::CREATE, solarSystem);

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

WorldModelAction SpaceWorldModel::CreateSpaceStation(shared_ptr<SolarSystem> homeSystem, NameType nameType) const {
    auto planets = homeSystem->Planets;
    float angle = rs->GetULongInRange(0, 3600) / 10.0f;
    float radians = static_cast<float>(angle * M_PI / 180);
    float x, y;
    if (planets.empty()) {
        float distanceToSun = param.planetDistanceBase + rs->GetNormalIntInRange(1, 5) * param.planetDistanceAverage;
        x = distanceToSun * cos(radians);
        y = distanceToSun * sin(radians);
    } else {
        auto planet = planets.at(rs->GetRandomIndex(planets.size()));
        int var = param.planetDistanceVariation / 5;
        int distanceVariation = rs->GetNormalIntInRange(-var, var);
        float distanceToPlanet = param.planetDistanceAverage / 5 + distanceVariation;
        x = planet->X + distanceToPlanet * cos(radians);
        y = planet->Y + distanceToPlanet * sin(radians);
    }

    int seed = rs->GetInt();
    string name = nameGenerator.CreateName(nameType, rs);
    return weave::WorldModelAction(WorldActionType::CREATE, make_shared<SpaceStation>(x, y, seed, name, homeSystem));
}

std::vector<WorldModelAction> SpaceWorldModel::InitializeNewWorld() const {
    vector<WorldModelAction> actions;

    // create a few solar systems
    vector<shared_ptr<SolarSystem>> solarSystems;
    for (int i = 0; i < param.startSystems; i++) {
        vector<WorldModelAction> solarSystemActions = CreateSolarSystem();
        auto lastAction = solarSystemActions.at(solarSystemActions.size() - 1).GetEntity();
        solarSystems.push_back(dynamic_pointer_cast<SolarSystem>(lastAction));
        std::move(solarSystemActions.begin(), solarSystemActions.end(), back_inserter(actions));
    }

    // maybe add a few space stations
    for (int i = 0; i < param.startSpaceStations; i++) {
        auto targetSystem = solarSystems.at(rs->GetRandomIndex(solarSystems.size()));
        actions.push_back(CreateSpaceStation(targetSystem));
    }

    // bury the rotting remains of dead civilizations
    for (int i = 0; i < param.startDeadCivs; i++) {
        actions.push_back(CreateDeadCivilization());
    }

    // create some good guys
    for (int i = 0; i < param.startFriends; i++) {
        auto newAgentAction = CreateAgent();
        actions.push_back(newAgentAction);

        MetaData metaData;
        metaData.SetValue("relationToPlayer", rs->GetNormalIntInRange(65, 100));
        actions.emplace_back(WorldActionType::UPDATE, newAgentAction.GetEntity(), metaData);
    }

    // Zapp Brannigan loves these guys
    for (int i = 0; i < param.startNeutrals; i++) {
        auto newAgentAction = CreateAgent();
        actions.push_back(newAgentAction);

        MetaData metaData;
        metaData.SetValue("relationToPlayer", rs->GetNormalIntInRange(35, 65));
        actions.emplace_back(WorldActionType::UPDATE, newAgentAction.GetEntity(), metaData);
    }

    // Some bad guys are needed too
    for (int i = 0; i < param.startEnemies; i++) {
        auto newAgentAction = CreateAgent();
        actions.push_back(newAgentAction);

        MetaData metaData;
        metaData.SetValue("relationToPlayer", rs->GetNormalIntInRange(0, 35));
        actions.emplace_back(WorldActionType::UPDATE, newAgentAction.GetEntity(), metaData);
    }

    return actions;
}
