//
// Created by michael on 27.10.15.
//

#include "../catch.hpp"
#include "../../Core/WeaverUtils.h"
#include "../../World/Space/SpaceWorldModel.h"

using namespace weave;
using namespace std;

TEST_CASE("Model Actions", "[model]") {
    shared_ptr<RandomStream> rs = make_shared<RandomStream>(43);
    SpaceWorldModel testModel(rs);
    shared_ptr<WorldEntity> entity = testModel.CreateLocation();
    REQUIRE(entity->GetId() == 0);

    SECTION("Empty list of actions") {
        vector<ModelAction> noActions;
        testModel.Execute(noActions);
        REQUIRE(testModel.GetEntities().size() == 0);
    }

    SECTION("CREATE model action") {
        vector<ModelAction> actions;
        ModelAction action(ActionType::CREATE, entity);
        actions.push_back(action);
        testModel.Execute(actions);
        REQUIRE(testModel.GetEntities().size() == 1);
        REQUIRE(entity->GetId() != 0);
    }

    SECTION("CREATE twice action") {
        vector<ModelAction> actions;
        ModelAction action(ActionType::CREATE, entity);
        actions.push_back(action);
        testModel.Execute(actions);
        REQUIRE_THROWS_AS(testModel.Execute(actions), ContractFailedException);
    }

    SECTION("KEEP unknown entity") {
        vector<ModelAction> actions;
        ModelAction action(ActionType::KEEP, entity);
        actions.push_back(action);
        REQUIRE_THROWS_AS(testModel.Execute(actions), ContractFailedException);
    }

    SECTION("create and keep entity") {
        vector<ModelAction> actions;
        ModelAction action(ActionType::CREATE, entity);
        actions.push_back(action);
        testModel.Execute(actions);
        actions[0] = ModelAction(ActionType::KEEP, entity);
        testModel.Execute(actions);
        REQUIRE(testModel.GetEntities().size() == 1);
        REQUIRE(entity->GetId() != 0);
    }

    SECTION("create and delete entity") {
        vector<ModelAction> actions;
        ModelAction action(ActionType::CREATE, entity);
        actions.push_back(action);
        testModel.Execute(actions);
        actions[0] = ModelAction(ActionType::DELETE, entity);
        testModel.Execute(actions);
        REQUIRE(testModel.GetEntities().size() == 0);
        REQUIRE(entity->GetId() == 0);
    }
}

TEST_CASE("Metadata", "[model]") {
    shared_ptr<RandomStream> rs = make_shared<RandomStream>(44);
    SpaceWorldModel testModel(rs);
    shared_ptr<WorldEntity> entity = testModel.CreateAgent();
    vector<ModelAction> actions;
    ModelAction action(ActionType::CREATE, entity);
    actions.push_back(action);
    testModel.Execute(actions);

    SECTION("Empty model metadata") {
        SpaceWorldModel emptyModel(rs);
        REQUIRE(emptyModel.GetMetaData(2).GetValueNames().size() == 0);
    }

    SECTION("Empty Metadata") {
        MetaData &metadata = testModel.GetMetaData(entity->GetId());
        REQUIRE(metadata.GetValueNames().size() == 0);
    }

    SECTION("Get does not create metadata") {
        MetaData &metadata = testModel.GetMetaData(entity->GetId());
        REQUIRE(!metadata.HasValue("Test123"));
        metadata.GetValue("Test123");
        REQUIRE(!metadata.HasValue("Test123"));
    }

    SECTION("Edit metadata simple") {
        MetaData &metadata = testModel.GetMetaData(entity->GetId());
        REQUIRE(!metadata.HasValue("Test123"));
        metadata.SetValue("Test123", 137);
        REQUIRE(metadata.HasValue("Test123"));
        REQUIRE(metadata.GetValue("Test123") == 137);
    }

    SECTION("delete entity with metadata") {
        ID id = entity->GetId();
        MetaData &metadata = testModel.GetMetaData(id);
        REQUIRE(!metadata.HasValue("Test123"));
        metadata.SetValue("Test123", 100);
        REQUIRE(metadata.HasValue("Test123"));
        actions[0] = ModelAction(ActionType::DELETE, entity);
        testModel.Execute(actions);
        REQUIRE(!testModel.GetMetaData(id).HasValue("Test123"));
    }
}
