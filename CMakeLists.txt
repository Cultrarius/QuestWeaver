cmake_minimum_required(VERSION 3.2)
project(QuestWeaver)

### Version 1 - Developer Version with Linux and gcc
if (CMAKE_BUILD_TYPE MATCHES Debug)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -O0 -fprofile-arcs -ftest-coverage -Wall -Wextra")

    file(COPY Resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
    file(COPY Test/Resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Test)
else ()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3")
endif ()

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMakeModules)
INCLUDE(CodeCoverage)
SETUP_TARGET_FOR_COVERAGE(Weaver_coverage QuestWeaverTest "${CMAKE_CURRENT_SOURCE_DIR}/coverage")
###

### Version 2 - Windows version (no gcc, no code coverage)
#[[
if (CMAKE_BUILD_TYPE MATCHES Debug)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -O0 -Wall -Wextra")

    file(COPY Resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
    file(COPY Test/Resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Test)
else ()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3")
endif ()
]]
###

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib")

include_directories(${PROJECT_SOURCE_DIR}/include)

set(SOURCE_FILES QuestWeaver.cpp Core/WeaverEngine.cpp Template/TemplateEngine.cpp QuestModel/QuestModel.cpp World/WorldModel.cpp
        QuestModel/Quest.cpp Template/QuestTemplate.cpp include/json/jsoncpp.cpp Template/Space/ExploreRegionTemplate.cpp
        Template/QuestTemplateFactory.cpp Template/Space/SpaceQuestTemplateFactory.cpp World/WorldEntity.cpp
        World/WorldModelAction.cpp World/Space/SpaceLocation.cpp World/Space/SpaceWorldModel.cpp QuestModel/Space/ExploreRegionQuest.cpp
        Core/WeaverUtils.cpp World/Space/SpaceAgent.cpp World/Space/SolarSystem.cpp World/MetaData.cpp Core/Graph/WeaverGraph.cpp
        Core/Graph/Node.cpp Core/Graph/Edge.cpp Core/Graph/GraphAnalyzer.cpp QuestModel/QuestModelAction.cpp
        World/WorldListener.cpp Story/StoryWriter.cpp Story/StoryTemplateFactory.cpp Story/StoryTemplate.cpp
        Story/Space/CommonSpaceStoryFactory.cpp Story/Nugget.cpp World/Space/SpaceNameGenerator.cpp
        include/Core/NameGen/TokenNameGenerator.cpp World/Space/Planet.cpp World/Space/DeadCivilization.cpp
        Template/Space/ScanPlanetTemplate.cpp QuestModel/Space/ScanPlanetQuest.cpp Template/QuestPropertyValue.cpp
        World/Space/Artifact.cpp World/Space/SpaceStation.cpp World/Space/SpaceWreck.cpp
        World/Space/SpaceShip.cpp QuestModel/Space/HuntAndKillQuest.cpp World/Space/MetaDataMarkers.cpp
        Template/Space/HuntAndKillTemplate.cpp Story/Space/ShipIntroStoryTemplate.cpp Core/WeaverTypes.cpp
        Story/SimpleStoryTemplate.cpp)

add_executable(QuestWeaverTest ${SOURCE_FILES} Test/catch.hpp Test/TestRunner.cpp Test/WeaverGraphTest.cpp
        Test/WeaverUtilsTest.cpp Test/Space/TemplateTest.cpp Test/Space/SerializationTest.cpp Test/Space/ModelTest.cpp
        Test/GraphAnalyzerTest.cpp Test/QuestModelTest.cpp Test/Mock/TestQuest.cpp Test/Mock/TestQuest.h
        Test/Mock/TestEntity.cpp Test/Mock/TestEntity.h Test/Mock/TestWorldListener.cpp Test/Mock/TestWorldListener.h
        Test/WeaverApiTest.cpp Test/StoryWriterTest.cpp Test/Mock/TestStoryTemplateFactory.h Test/Mock/TestStoryTemplate.h
        Test/Mock/TestHelper.h Test/Mock/TestHelper.cpp Test/Mock/TestQuestTemplate.cpp Test/Mock/TestQuestTemplate.h
        Test/Mock/TestQuestTemplateFactory.cpp Test/Mock/TestQuestTemplateFactory.h Test/Mock/ProxyQuestFactory.cpp Test/Mock/ProxyQuestFactory.h)

add_library(QuestWeaver STATIC ${SOURCE_FILES})
