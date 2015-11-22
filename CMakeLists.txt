cmake_minimum_required(VERSION 3.2)
project(QuestWeaver)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -O0 -Wall")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

include_directories(./serialization)

set(SOURCE_FILES QuestWeaver.cpp QuestWeaver.h Core/WeaverEngine.cpp Core/WeaverEngine.h Template/TemplateEngine.cpp Template/TemplateEngine.h QuestModel/QuestModel.cpp QuestModel/QuestModel.h World/WorldModel.cpp World/WorldModel.h QuestModel/Quest.cpp QuestModel/Quest.h Template/Template.cpp Template/Template.h json/json.h json/json-forwards.h json/jsoncpp.cpp Template/Space/ExploreRegionTemplate.cpp Template/Space/ExploreRegionTemplate.h Template/TemplateFactory.cpp Template/TemplateFactory.h Template/Space/SpaceQuestTemplateFactory.cpp Template/Space/SpaceQuestTemplateFactory.h World/WorldEntity.cpp World/WorldEntity.h World/WorldModelAction.cpp World/WorldModelAction.h World/Space/SpaceLocation.cpp World/Space/SpaceLocation.h World/Space/SpaceWorldModel.cpp World/Space/SpaceWorldModel.h QuestModel/Space/ExploreRegionQuest.cpp QuestModel/Space/ExploreRegionQuest.h Core/WeaverUtils.h Core/WeaverUtils.cpp World/Space/SpaceAgent.cpp World/Space/SpaceAgent.h World/Space/SolarSystem.cpp World/Space/SolarSystem.h Core/WeaverTypes.h serialization/cereal.h World/MetaData.cpp World/MetaData.h Core/Graph/WeaverGraph.cpp Core/Graph/WeaverGraph.h Core/Graph/Node.cpp Core/Graph/Node.h Core/Graph/Edge.cpp Core/Graph/Edge.h Core/GraphAnalyzer.cpp Core/GraphAnalyzer.h QuestModel/QuestModelAction.cpp QuestModel/QuestModelAction.h)
add_executable(QuestWeaverExecutable main.cpp ${SOURCE_FILES})
add_executable(QuestWeaverTest ${SOURCE_FILES} Test/catch.hpp Test/TestRunner.cpp Test/WeaverGraphTest.cpp Test/WeaverUtilsTest.cpp Test/Space/TemplateTest.cpp Test/Space/SerializationTest.cpp Test/Space/ModelTest.cpp Test/GraphAnalyzerTest.cpp Test/QuestModelTest.cpp Test/Mock/TestQuest.cpp Test/Mock/TestQuest.h Test/Mock/TestEntity.cpp Test/Mock/TestEntity.h)
add_library(QuestweaverLibrary ${SOURCE_FILES})
