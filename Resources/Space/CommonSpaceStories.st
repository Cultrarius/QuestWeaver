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
