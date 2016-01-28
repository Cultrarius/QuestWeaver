cmake_minimum_required(VERSION 3.2)
project(QuestWeaver)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -g -O0 -Wall")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib")

include_directories(./include)

set(HEADER_FILES QuestWeaver.h include/Core/WeaverEngine.h include/Template/TemplateEngine.h include/QuestModel/QuestModel.h
        include/World/WorldModel.h include/QuestModel/Quest.h include/Template/Template.h include/json/json.h include/json/json-forwards.h
        include/Template/Space/ExploreRegionTemplate.h include/Template/TemplateFactory.h include/Template/Space/SpaceQuestTemplateFactory.h
        include/World/WorldEntity.h include/World/WorldModelAction.h include/World/Space/SpaceLocation.h include/World/Space/SpaceWorldModel.h
        include/QuestModel/Space/ExploreRegionQuest.h include/Core/WeaverUtils.h include/World/Space/SpaceAgent.h
        include/World/Space/SolarSystem.h include/Core/WeaverTypes.h include/cereal.h include/World/MetaData.h
        include/Core/Graph/WeaverGraph.h include/Core/Graph/Node.h include/Core/Graph/Edge.h include/Core/Graph/GraphAnalyzer.h
        include/QuestModel/QuestModelAction.h include/World/WorldListener.h include/Story/StoryWriter.h include/WeaverConfig.h)

set(SOURCE_FILES QuestWeaver.cpp Core/WeaverEngine.cpp Template/TemplateEngine.cpp QuestModel/QuestModel.cpp World/WorldModel.cpp
        QuestModel/Quest.cpp Template/Template.cpp include/json/jsoncpp.cpp Template/Space/ExploreRegionTemplate.cpp
        Template/TemplateFactory.cpp Template/Space/SpaceQuestTemplateFactory.cpp World/WorldEntity.cpp
        World/WorldModelAction.cpp World/Space/SpaceLocation.cpp World/Space/SpaceWorldModel.cpp QuestModel/Space/ExploreRegionQuest.cpp
        Core/WeaverUtils.cpp World/Space/SpaceAgent.cpp World/Space/SolarSystem.cpp World/MetaData.cpp Core/Graph/WeaverGraph.cpp
        Core/Graph/Node.cpp Core/Graph/Edge.cpp Core/Graph/GraphAnalyzer.cpp QuestModel/QuestModelAction.cpp
        World/WorldListener.cpp Story/StoryWriter.cpp)
add_executable(QuestWeaverExecutable main.cpp ${SOURCE_FILES})
add_executable(QuestWeaverTest ${SOURCE_FILES} Test/catch.hpp Test/TestRunner.cpp Test/WeaverGraphTest.cpp
        Test/WeaverUtilsTest.cpp Test/Space/TemplateTest.cpp Test/Space/SerializationTest.cpp Test/Space/ModelTest.cpp
        Test/GraphAnalyzerTest.cpp Test/QuestModelTest.cpp Test/Mock/TestQuest.cpp Test/Mock/TestQuest.h
        Test/Mock/TestEntity.cpp Test/Mock/TestEntity.h Test/Mock/TestWorldListener.cpp Test/Mock/TestWorldListener.h
        Test/WeaverApiTest.cpp)
add_library(QuestWeaver ${SOURCE_FILES})
