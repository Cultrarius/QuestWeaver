[
  {
    "key": "nuggetB",
    "requiredTypes": [],
    "texts": [
      "B."
    ]
  },
  {
    "key": "nuggetTestEntity",
    "requiredTypes": [
      "TestEntityType"
    ],
    "texts": [
      "a %TestEntityType"
    ]
  },
  {
    "key": "nuggetMissingReplacement",
    "requiredTypes": [
      "TestEntityType"
    ],
    "texts": [
      "a %TestXEntityXType"
    ]
  },
  {
    "key": "complexNugget",
    "requiredTypes": [
      "TestEntityType",
      "location"
    ],
    "texts": [
      "a %TestEntityType at %location"
    ]
  },
  {
    "key": "nuggetTestWrongType",
    "requiredTypes": [
      "NotMyType"
    ],
    "texts": [
      "This is %NotMyType"
    ]
  }
]