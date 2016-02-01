cmake_minimum_required(VERSION 3.2)
project(QuestWeaver)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -g -O0 -Wall")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib")

include_directories(./include)

set(SOURCE_FILES QuestWeaver.cpp Core/WeaverEngine.cpp Template/TemplateEngine.cpp QuestModel/QuestModel.cpp World/WorldModel.cpp
        QuestModel/Quest.cpp Template/QuestTemplate.cpp include/json/jsoncpp.cpp Template/Space/ExploreRegionTemplate.cpp
        Template/QuestTemplateFactory.cpp Template/Space/SpaceQuestTemplateFactory.cpp World/WorldEntity.cpp
        World/WorldModelAction.cpp World/Space/SpaceLocation.cpp World/Space/SpaceWorldModel.cpp QuestModel/Space/ExploreRegionQuest.cpp
        Core/WeaverUtils.cpp World/Space/SpaceAgent.cpp World/Space/SolarSystem.cpp World/MetaData.cpp Core/Graph/WeaverGraph.cpp
        Core/Graph/Node.cpp Core/Graph/Edge.cpp Core/Graph/GraphAnalyzer.cpp QuestModel/QuestModelAction.cpp
        World/WorldListener.cpp Story/StoryWriter.cpp Story/StoryTemplateFactory.cpp Story/StoryTemplate.cpp
        Story/Space/CommonSpaceStoryFactory.cpp)
add_executable(QuestWeaverExecutable main.cpp ${SOURCE_FILES})
add_executable(QuestWeaverTest ${SOURCE_FILES} Test/catch.hpp Test/TestRunner.cpp Test/WeaverGraphTest.cpp
        Test/WeaverUtilsTest.cpp Test/Space/TemplateTest.cpp Test/Space/SerializationTest.cpp Test/Space/ModelTest.cpp
        Test/GraphAnalyzerTest.cpp Test/QuestModelTest.cpp Test/Mock/TestQuest.cpp Test/Mock/TestQuest.h
        Test/Mock/TestEntity.cpp Test/Mock/TestEntity.h Test/Mock/TestWorldListener.cpp Test/Mock/TestWorldListener.h
        Test/WeaverApiTest.cpp)
add_library(QuestWeaver ${SOURCE_FILES})
