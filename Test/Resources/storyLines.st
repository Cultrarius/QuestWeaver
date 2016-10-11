[
  {
    "key": "simpleLine",
    "type": "complex",
    "required": [],
    "text": "A. [nuggetB] C"
  },
  {
    "key": "entityLine",
    "type": "complex",
    "required": [
      "TestEntityType"
    ],
    "text": "I wish me [nuggetTestEntity:1] to play with."
  },
  {
    "key": "entityLineSimple",
    "type": "simple",
    "conditions": ["once per entity", "does not have someProperty"],
    "required": [
      "TestEntityType"
    ],
    "text": "I wish me [nuggetTestEntity:1] to play with."
  },
  {
    "key": "entityLineSimple2",
    "type": "simple",
    "conditions": ["does have someProperty"],
    "required": [
      "TestEntityType"
    ],
    "text": "I wish me [nuggetTestEntity:1] to play with."
  },
  {
    "key": "wrongType",
    "type": "wrongType",
    "required": [
      "TestEntityType"
    ],
    "text": "I wish me [nuggetTestEntity:1] to play with."
  },
  {
    "key": "conditionGreaterAndSmaller",
    "type": "simple",
    "conditions": ["someProperty > 15", "someProperty < 40"],
    "required": [
      "TestEntityType"
    ],
    "text": "I wish me [nuggetTestEntity:1] to play with."
  },
  {
    "key": "unknownCondition",
    "type": "simple",
    "conditions": ["unknown stuff"],
    "required": [
      "TestEntityType"
    ],
    "text": "I wish me [nuggetTestEntity:1] to play with."
  },
  {
    "key": "missingReplacement",
    "type": "complex",
    "required": [
      "TestEntityType"
    ],
    "text": "I wish me [nuggetMissingReplacement:1]."
  },
  {
    "key": "wrongType",
    "type": "complex",
    "required": [
      "TestEntityType"
    ],
    "text": "I wish me [nuggetTestWrongType:1] to play with."
  },
  {
    "key": "stringOnlyLines",
    "type": "complex",
    "required": [],
    "text": "This is great! I really love this."
  },
  {
    "key": "unknownNugget",
    "type": "complex",
    "required": [],
    "text": "X [thisIsSomeNugget] Z"
  },
  {
    "key": "wrongNuggetIds",
    "type": "complex",
    "required": [],
    "text": "X [complexNugget] Z"
  },
  {
    "key": "wrongNuggetContent",
    "type": "complex",
    "required": [
      "TestEntityType"
    ],
    "text": "X [nuggetTestWrongType] Z"
  }
]