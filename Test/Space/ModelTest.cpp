//
// Created by michael on 27.10.15.
//

#include "../catch.hpp"
#include "../../Core/WeaverUtils.h"
#include "../../World/Space/SpaceWorldModel.h"

using namespace weave;
using namespace std;

TEST_CASE("World Model", "[model]") {
    shared_ptr<RandomStream> rs = make_shared<RandomStream>(43);
    SpaceWorldModel testModel(rs);
    shared_ptr<WorldEntity> entity = testModel.CreateLocation();
    REQUIRE(entity->GetId() == 0);

    SECTION("Checking emptty list of actions") {
        vector<ModelAction> noActions;
        testModel.Execute(noActions);
        REQUIRE(testModel.GetEntities().size() == 0);
    }

    SECTION("Checking CREATE model action") {
        vector<ModelAction> actions;
        ModelAction action(ActionType::CREATE, entity);
        actions.push_back(action);
        testModel.Execute(actions);
        REQUIRE(testModel.GetEntities().size() == 1);
        REQUIRE(entity->GetId() != 0);
    }

    SECTION("Checking CREATE twice action") {
        vector<ModelAction> actions;
        ModelAction action(ActionType::CREATE, entity);
        actions.push_back(action);
        testModel.Execute(actions);
        REQUIRE_THROWS_AS(testModel.Execute(actions), ContractFailedException);
    }

    SECTION("Checking KEEP unknown entity") {
        vector<ModelAction> actions;
        ModelAction action(ActionType::KEEP, entity);
        actions.push_back(action);
        REQUIRE_THROWS_AS(testModel.Execute(actions), ContractFailedException);
    }

    SECTION("Checking create and keep entity") {
        vector<ModelAction> actions;
        ModelAction action(ActionType::CREATE, entity);
        actions.push_back(action);
        testModel.Execute(actions);
        actions[0] = ModelAction(ActionType::KEEP, entity);
        testModel.Execute(actions);
        REQUIRE(testModel.GetEntities().size() == 1);
        REQUIRE(entity->GetId() != 0);
    }
}
