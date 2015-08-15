cmake_minimum_required(VERSION 3.2)
project(QuestWeaver)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp)
add_executable(QuestWeaver ${SOURCE_FILES} QuestWeaver.cpp QuestWeaver.h Core/WeaverEngine.cpp Core/WeaverEngine.h Template/TemplateEngine.cpp Template/TemplateEngine.h QuestModel/QuestModel.cpp QuestModel/QuestModel.h World/WorldModel.cpp World/WorldModel.h QuestModel/Quest.cpp QuestModel/Quest.h Template/Template.cpp Template/Template.h json/json.h json/json-forwards.h json/jsoncpp.cpp Template/Space/ExploreRegionTemplate.cpp Template/Space/ExploreRegionTemplate.h Template/TemplateFactory.cpp Template/TemplateFactory.h Template/Space/SpaceQuestTemplateFactory.cpp Template/Space/SpaceQuestTemplateFactory.h)