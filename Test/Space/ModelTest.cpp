//
// Created by michael on 27.10.15.
//

#include "../catch.hpp"
#include "../../Core/WeaverUtils.h"
#include "../../World/Space/SpaceWorldModel.h"
#include "../Mock/TestWorldListener.h"

using namespace weave;
using namespace std;

TEST_CASE("Model Actions", "[model]") {
    shared_ptr<RandomStream> rs = make_shared<RandomStream>(43);
    shared_ptr<TestWorldListener> listener = make_shared<TestWorldListener>();
    SpaceWorldModel testModel(rs);
    testModel.AddListener(listener);
    shared_ptr<WorldEntity> entity = testModel.CreateLocation();
    REQUIRE(entity->GetId() == 0);

    SECTION("Empty list of actions") {
        vector<WorldModelAction> noActions;
        testModel.Execute(noActions);
        REQUIRE(testModel.GetEntities().size() == 0);
        SECTION("Empty listener") {
            REQUIRE(listener->calledActions.size() == 0);
        }
    }

    SECTION("CREATE model action") {
        vector<WorldModelAction> actions;
        WorldModelAction action(WorldActionType::CREATE, entity);
        actions.push_back(action);
        testModel.Execute(actions);
        REQUIRE(testModel.GetEntities().size() == 1);
        REQUIRE(entity->GetId() != 0);
        SECTION("Single listener action") {
            REQUIRE(listener->calledActions.size() == 1);
        }
    }

    SECTION("CREATE twice action") {
        vector<WorldModelAction> actions;
        WorldModelAction action(WorldActionType::CREATE, entity);
        actions.push_back(action);
        testModel.Execute(actions);
        REQUIRE_THROWS_AS(testModel.Execute(actions), ContractFailedException);
    }

    SECTION("KEEP known entity") {
        vector<WorldModelAction> actions;
        WorldModelAction action(WorldActionType::CREATE, entity);
        actions.push_back(action);
        testModel.Execute(actions);
        actions[0] = WorldModelAction(WorldActionType::KEEP, entity);
        testModel.Execute(actions);
    }

    SECTION("KEEP unknown entity without id") {
        vector<WorldModelAction> actions;
        WorldModelAction action(WorldActionType::KEEP, entity);
        actions.push_back(action);
        REQUIRE_THROWS_AS(testModel.Execute(actions), ContractFailedException);
    }

    SECTION("UPDATE unknown entity") {
        vector<WorldModelAction> actions;
        WorldModelAction action(WorldActionType::UPDATE, entity);
        actions.push_back(action);
        REQUIRE_THROWS_AS(testModel.Execute(actions), ContractFailedException);
    }

    SECTION("UPDATE unknown entity with id") {
        vector<WorldModelAction> actions;
        WorldModelAction action(WorldActionType::CREATE, entity);
        actions.push_back(action);
        testModel.Execute(actions);
        actions[0] = WorldModelAction(WorldActionType::UPDATE, entity);
        SpaceWorldModel otherModel(rs);
        REQUIRE_THROWS_AS(otherModel.Execute(actions), ContractFailedException);
    }

    SECTION("create and keep entity") {
        vector<WorldModelAction> actions;
        WorldModelAction action(WorldActionType::CREATE, entity);
        actions.push_back(action);
        testModel.Execute(actions);
        actions[0] = WorldModelAction(WorldActionType::KEEP, entity);
        testModel.Execute(actions);
        REQUIRE(testModel.GetEntities().size() == 1);
        REQUIRE(entity->GetId() != 0);
    }

    SECTION("create and delete entity") {
        vector<WorldModelAction> actions;
        WorldModelAction action(WorldActionType::CREATE, entity);
        actions.push_back(action);
        testModel.Execute(actions);
        actions[0] = WorldModelAction(WorldActionType::DELETE, entity);
        testModel.Execute(actions);
        REQUIRE(testModel.GetEntities().size() == 0);
        REQUIRE(entity->GetId() == 0);
        SECTION("Two listener actions") {
            REQUIRE(listener->calledActions.size() == 2);
            REQUIRE(listener->calledActions[0].GetActionType() == WorldActionType::CREATE);
            REQUIRE(listener->calledActions[1].GetActionType() == WorldActionType::DELETE);
        }
    }
}

TEST_CASE("Metadata", "[model]") {
    shared_ptr<RandomStream> rs = make_shared<RandomStream>(44);
    SpaceWorldModel testModel(rs);
    shared_ptr<WorldEntity> entity = testModel.CreateAgent();
    vector<WorldModelAction> actions;
    WorldModelAction action(WorldActionType::CREATE, entity);
    actions.push_back(action);
    testModel.Execute(actions);

    SECTION("Empty metadata test") {
        MetaData metaData;
        REQUIRE(metaData.GetValueNames().size() == 0);
        REQUIRE(metaData.GetValue("Test123") == 0);
        REQUIRE(!metaData.HasValue("Test123"));
    }

    SECTION("Metadata set and get test") {
        MetaData metaData;
        metaData.SetValue("Test123", 42);
        metaData.SetValue("Test456", 43);
        REQUIRE(metaData.GetValueNames().size() == 2);
        REQUIRE(metaData.HasValue("Test123"));
        REQUIRE(metaData.HasValue("Test456"));
        REQUIRE(metaData.GetValue("Test123") == 42);
        REQUIRE(metaData.GetValue("Test456") == 43);
    }

    SECTION("Empty model metadata") {
        SpaceWorldModel emptyModel(rs);
        REQUIRE(emptyModel.GetMetaData(2).GetValueNames().size() == 0);
    }

    SECTION("Empty Metadata") {
        MetaData metadata = testModel.GetMetaData(entity->GetId());
        REQUIRE(metadata.GetValueNames().size() == 0);
    }

    SECTION("Get does not create metadata") {
        MetaData metadata = testModel.GetMetaData(entity->GetId());
        REQUIRE(!metadata.HasValue("Test123"));
        metadata.GetValue("Test123");
        REQUIRE(!metadata.HasValue("Test123"));
    }

    SECTION("Edit metadata immutablility") {
        MetaData metadata = testModel.GetMetaData(entity->GetId());
        REQUIRE(!metadata.HasValue("Test123"));
        metadata.SetValue("Test123", 137);
        REQUIRE(metadata.HasValue("Test123"));
        REQUIRE(!testModel.GetMetaData(entity->GetId()).HasValue("Test123"));
    }

    SECTION("Create metadata entry") {
        SpaceWorldModel metaDataModel(rs);
        shared_ptr<WorldEntity> metaEntity = testModel.CreateLocation();
        MetaData metaData;
        metaData.SetValue("Test123", 142);
        actions[0] = WorldModelAction(WorldActionType::CREATE, metaEntity, metaData);
        metaDataModel.Execute(actions);
        MetaData modelData = metaDataModel.GetMetaData(metaEntity->GetId());
        REQUIRE(modelData.HasValue("Test123"));
        REQUIRE(modelData.GetValue("Test123") == 142);
    }

    SECTION("delete entity with metadata") {
        SpaceWorldModel metaDataModel(rs);
        shared_ptr<WorldEntity> metaEntity = testModel.CreateLocation();
        MetaData metaData;
        metaData.SetValue("Test123", 142);
        actions[0] = WorldModelAction(WorldActionType::CREATE, metaEntity, metaData);
        metaDataModel.Execute(actions);
        ID id = metaEntity->GetId();
        REQUIRE(metaDataModel.GetMetaData(id).HasValue("Test123"));
        actions[0] = WorldModelAction(WorldActionType::DELETE, metaEntity);
        metaDataModel.Execute(actions);
        REQUIRE(!metaDataModel.GetMetaData(id).HasValue("Test123"));
    }

    SECTION("update entity with metadata") {
        SpaceWorldModel metaDataModel(rs);
        shared_ptr<WorldEntity> metaEntity = testModel.CreateLocation();
        MetaData metaData;
        metaData.SetValue("Test123", 142);
        actions[0] = WorldModelAction(WorldActionType::CREATE, metaEntity, metaData);
        metaDataModel.Execute(actions);
        ID id = metaEntity->GetId();
        REQUIRE(metaDataModel.GetMetaData(id).GetValue("Test123") == 142);
        metaData.SetValue("Test123", 191);
        actions[0] = WorldModelAction(WorldActionType::UPDATE, metaEntity, metaData);
        metaDataModel.Execute(actions);
        REQUIRE(metaDataModel.GetMetaData(id).GetValue("Test123") == 191);
    }

    SECTION("update metadata key addition") {
        SpaceWorldModel metaDataModel(rs);
        shared_ptr<WorldEntity> metaEntity = testModel.CreateLocation();
        MetaData metaData;
        metaData.SetValue("Test123", 142);
        actions[0] = WorldModelAction(WorldActionType::CREATE, metaEntity, metaData);
        metaDataModel.Execute(actions);
        ID id = metaEntity->GetId();
        REQUIRE(!metaDataModel.GetMetaData(id).HasValue("Test456"));
        metaData.SetValue("Test456", 191);
        actions[0] = WorldModelAction(WorldActionType::UPDATE, metaEntity, metaData);
        metaDataModel.Execute(actions);
        REQUIRE(metaDataModel.GetMetaData(id).GetValue("Test123") == 142);
        REQUIRE(metaDataModel.GetMetaData(id).GetValue("Test456") == 191);
    }

    SECTION("empty update must not delete keys") {
        SpaceWorldModel metaDataModel(rs);
        shared_ptr<WorldEntity> metaEntity = testModel.CreateLocation();
        MetaData metaData;
        metaData.SetValue("Test123", 142);
        actions[0] = WorldModelAction(WorldActionType::CREATE, metaEntity, metaData);
        metaDataModel.Execute(actions);
        ID id = metaEntity->GetId();
        REQUIRE(metaDataModel.GetMetaData(id).GetValue("Test123") == 142);
        actions[0] = WorldModelAction(WorldActionType::UPDATE, metaEntity, MetaData());
        metaDataModel.Execute(actions);
        REQUIRE(metaDataModel.GetMetaData(id).GetValue("Test123") == 142);
    }
}
