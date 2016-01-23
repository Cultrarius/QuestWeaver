# QuestWeaver <a href="https://travis-ci.org/Cultrarius/QuestWeaver"><img src="https://travis-ci.org/Cultrarius/QuestWeaver.svg"></a> [![Coverage Status](https://coveralls.io/repos/Cultrarius/QuestWeaver/badge.svg?branch=master&service=github)](https://coveralls.io/github/Cultrarius/QuestWeaver?branch=master)
Procedurally generated quests and stories for computer games.

This project is still in its early stages and under heavy development!

This project includes the following JSON parser: https://github.com/open-source-parsers/jsoncpp

## Usage

The quest system of a game is not an isolated part of the system as it is heavily involved in the current world state and events.
Therefore, the game has to provide the quest system with the necessary information about the game's state and has to
fulfill change requests made by the quest system (otherwise the state will be inconsistent).

To use the QuestWeaver system, you have to fulfill the following prequisites:

* Implement and register custom `TemplateFactory` classes that read your custom template files
* Implement a custom `WorldModel` class
 
```
// Configure the quest system
WeaverConfig config;
config.dirs.modDirectory = "./Template/";
QuestWeaver weaver(config);
shared_ptr<Quest> newQuest = weaver.CreateNewQuest();
```

## Features

* Fully portable, serializable quest system, perfect for use in savegames. Supports JSON and a compact binary format. 
* Separates quest logic from quest data (such as titles/descriptions) by using template files
* Uses a complex weighted graph search to create new quests.
This results in more interesting and coherent quest stories as world actors and entities are reused when fitting.
* Supports mod directories to overwrite quest templates
* The game world entities to be used in quests are supplied directly from a game's custom world model