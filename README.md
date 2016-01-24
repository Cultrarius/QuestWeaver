# QuestWeaver <a href="https://travis-ci.org/Cultrarius/QuestWeaver"><img src="https://travis-ci.org/Cultrarius/QuestWeaver.svg"></a> [![Coverage Status](https://coveralls.io/repos/Cultrarius/QuestWeaver/badge.svg?branch=master&service=github)](https://coveralls.io/github/Cultrarius/QuestWeaver?branch=master)
Procedurally generated quests and stories for computer games.

This project is still in its early stages and under heavy development!

This project includes the following JSON parser: https://github.com/open-source-parsers/jsoncpp

## Features

* Fully *portable*, serializable quest system, perfect for use in savegames. Supports JSON and a compact binary format. 
* Separates quest logic from quest data (such as titles/descriptions) by using *template files*
* Uses a complex *weighted graph search* to create new quests.
This results in more interesting and coherent quest stories as world actors and entities are reused when fitting.
* Supports *mod directories* to overwrite quest templates
* The game world entities to be used in quests are supplied directly from a game's *custom world model*

## Usage

The quest system of a game is not an isolated part of the system as it is heavily involved in the current world state and events.
Therefore, the game has to provide the quest system with the necessary information about the game's state and has to
fulfill change requests made by the quest system (otherwise the state will be inconsistent).

To use the QuestWeaver system, you have to follow these steps:

* Implement and register custom `TemplateFactory` classes that read your custom template files
* Implement a custom `WorldModel` class
* Register your wold model and template factories with the `QuestWeaver` instance
 
```cpp
QuestWeaver Init() {
    // Create your template factories
    shared_ptr<TemplateFactory> factory = make_shared<MyQuestTemplateFactory>();
    
    // Create your world model
    auto worldModel = new MyWorldModel();
    shared_ptr<WorldListener> myWorldListener = make_shared<MyWorldListener>();
    
    // Create Configuration
    WeaverConfig config;
    config.worldModel = worldModel;
    config.dirs.modDirectory = "./Template/";
    
    // Create the quest system
    QuestWeaver weaver(config);
    weaver.RegisterTemplateFactory(factory);
    weaver.GetWorldModel().AddListener(myWorldListener);
    
    return weaver;
}

// Create new quests
shared_ptr<Quest> newQuest = weaver.CreateNewQuest();
```


Quests usually want to change the world state as well as their own state (e.g. when a quest succeeds).
To do that, you can tick the quest system in your game loop and it will take care of the rest by ticking each quest
separately.
The `WorldListener` allows you to change the game state whenever the world model was changed by the quest system.

```cpp
while (true) {
   // game loop ...
   
   weaver.Tick(delta); // updates the world state 
}
```


You can also fully serialize the quest system, e.g. to create a save game or to debug it:

```cpp
void SerialTest() {
    QuestWeaver weaver = Init();
    
    // serialize
    stringstream ss; // can also be a file stream
    weaver.Serialize(ss, StreamType::JSON);
    cout << ss.str() << endl; // or save it to disk
    
    // deserialize - it is important to re-register the template factories and the world model listener!
    QuestWeaver deserialized = QuestWeaver::Deserialize(ss, StreamType::JSON, config.dirs);
    weaver.RegisterTemplateFactory(factory);
    weaver.GetWorldModel().AddListener(myWorldListener);
}
```