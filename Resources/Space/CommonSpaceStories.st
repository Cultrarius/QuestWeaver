// allowed required entities:
// artifact, deadCivilization, planet, solarSystem, agent, location, spaceShip, spaceStation, spaceWreck

// allowed "conditions":
// * once per entity
// * does not have 'property'
// * 'property' greater 'value' (e.g. relationToPlayer greater 20)
// * 'property' smaller 'value'
[
  {
    "key": "agentIntro",
    "required": ["agent"],
    "conditions": ["once per entity", "relationToPlayer greater 20"],
    "text": "[actorIntroFriendly:1]
    {actorUnknownOrigin:1}
    {agentAge:1}{agentHobby:1}

    {logEntry}"
  },
  {
    "key": "agentIntroSerious",
    "required": ["agent"],
    "conditions": ["once per entity"],
    "text": "[actorIntroMysterious|actorIntroMachine:1]
    {actorUnknownOrigin:1}
    {agentAge:1}{agentHobby:1}

    {logEntry}"
  },
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
