// allowed required entities:
// artifact, deadCivilization, planet, solarSystem, agent, location, spaceShip, spaceStation, spaceWreck

// allowed conditions:
// once per entity
// 'property' greater 'value' (e.g. relationToPlayer greater 20)
// 'property' smaller 'value'
// does not have 'property'
[
  {
    "key": "agentIntro",
    "required": ["agent"],
    "conditions": ["once per entity"],
    "text": "[actorIntroMysterious|actorIntroFriendly|actorIntroMachine:1]
    {actorUnknownOrigin:1}
    {agentAge:1}{agentHobby:1}

    {logEntry}"
  },
  // new serious agent intro story
  {
    "key": "locationIntro",
    "required": ["solarSystem"],
    "conditions": ["once per entity"],
    "text": "[locationStrangeGravity|locationBattle:1]

    {logEntry}"
  },
  {
    "key": "planetIntro",
    "required": ["planet"],
    "conditions": ["once per entity"],
    "text": "[planetVeryOld|planetVeryYoung|planetCuddlyCreatures:1]

    {planetMetals}
    {planetLife}
    {logEntry}"
  },
  {
    "key": "shipIntro",
    "type": "complex",
    "required": ["spaceShip"],
    "conditions": ["once per entity"],
    "text": "[shipOriginOwner:shipAndOwner]

    {logEntry}"
  }
]
