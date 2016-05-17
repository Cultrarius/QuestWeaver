[
  {
    "key": "empty",
    "requiredTypes": [],
    "texts": [""]
  },
  {
    "key": "actorIntroMysterious",
    "requiredTypes": ["agent"],
    "texts": ["%agent is a mysterious being roaming the galaxy in search of a lost soul."]
  },
  {
    "key": "actorIntroMachine",
    "requiredTypes": ["agent"],
    "texts": [
      "%agent appears to be a sentient machine, which outlived its creators.",
      "%agent appears to be a sentient machine, but the creator is unknown."
    ]
  },
  {
    "key": "actorUnknownOrigin",
    "requiredTypes": ["agent"],
    "texts": [
      "It is not known where %agent originated from.",
      "It is not known where %agent originated from, but several clues suggest that the location lies beyond our known galaxy.",
      "It is not known where %agent originated from, but several clues suggest that the location lies in our known galaxy.",
      "Our databanks do not contain any information on the origin of %agent.",
      "Our databanks do not contain any conclusive information on the origin of %agent.",
      "Our databanks do not contain any information on the origin of %agent. It looks like someone recently deleted that information, which is rather strange."
    ]
  },
  {
    "key": "logEntry",
    "requiredTypes": [],
    "randomizedValue": [
      {
        "key": "entryNumber",
        "min": 1000000,
        "max": 10000000
      }
    ],
    "texts": [
      "\n\n[End of datalog entry %entryNumber]",
      "\n\n[Excerpt from datalog entry %entryNumber]",
      "\n\n[Datalog entry %entryNumber]",
      "\n\n[Warning: datalog entry corrupted]"
    ]
  },
  {
    "key": "locationStrangeGravity",
    "requiredTypes": [
      "solarSystem"
    ],
    "texts": [
      "The star system known as %solarSystem was only discovered very recently. Due to an unknown phenomenon, it has an unusual amount of gravity lenses positioned around it, hiding it from cursory sensor scans.",
      "The star system %solarSystem produced such strange gravitational readings, that the ship first reported malfunctioning sensors. The cause of this is still unknown."
    ]
  },
  {
    "key": "locationBattle",
    "requiredTypes": [
      "solarSystem"
    ],
    "randomizedName": [
      {
        "key": "shipName",
        "type": "ship"
      }
    ],
    "texts": [
      "Space around the star %solarSystem is littered with debris. Is is very likely that this system once saw a battle of enormous magnitude.",
      "Space around the star %solarSystem is littered with debris. It looks like a big battle was fought here not too long ago.",
      "All throughout the star system %solarSystem one can find the wrecks of gigantic ships from a battle that was fought ages ago.",
      "All throughout the star system %solarSystem one can find the wrecks of gigantic ships from a battle that was fought ages ago. One of the ships still broadcasts its name into the void: %shipName",
      "%shipName - one of the many ship names that lie buried in the graveyard that is star system %solarSystem. A massive battle was fought here in the past and the fallout is still visible to your sensors."
    ]
  },
  {
    "key": "planetVeryOld",
    "requiredTypes": [
      "planet"
    ],
    "texts": [
      "Scans indicate that the planet %planet is extremely old and predates even the creation of its own solar system, as paradoxical as it sounds.",
      "Scans indicate that the planet %planet is extremely old and predates even the creation of its own solar system. This means it is either the result of a massive collision or the creation of an unknown entity.",
      "Our sensors show that planet %planet is extremely old and might very well be one of the first planets ever to form under the light of this universe."
    ]
  },
  {
    "key": "planetVeryYoung",
    "requiredTypes": [
      "planet"
    ],
    "texts": [
      "Scans indicate that the planet %planet is extremely young for a solar system of this age and size. Since parts of the data are corrupt, this might just be a sensor glitch.",
      "Scans indicate that the planet %planet is extremely young for a solar system of this age and size. It is very likely that the planet is the result of a collision between two massive interstellar objects.",
      "Scans indicate that the planet %planet is extremely young for a solar system of this age and size. It is possible that the planet is the recent creation of an unknown civilization or being."
    ]
  },
  {
    "key": "planetMetals",
    "requiredTypes": [],
    "randomizedValue": [
      {
        "key": "percent1",
        "min": 10,
        "max": 40
      },
      {
        "key": "percent2",
        "min": 10,
        "max": 40
      }
    ],
    "texts": [
      "\n[Metal scan result]: no significant amount of valuable metals in the planet's crust.",
      "\n[Metal scan result]: no amount of valuable metals in the planet's crust.",
      "\n[Metal scan result]: no valuable metals in the planet's crust.",
      "\n[Metal scan result]: no metal at all, since the planet is a gas giant.",
      "\n[Metal scan result]: High amount of Hg (%percent1%) and Ur salts (%percent2%).",
      "\n[Metal scan result]: High amount of raw, liquid Hg (%percent1%).",
      "\n[Metal scan result]: High amount of Al (%percent1%) and Mo (%percent2%).",
      "\n[Metal scan result]: High amount of Ge (%percent1%) and Ur (%percent2%)."
    ]
  },
  {
    "key": "planetLife",
    "requiredTypes": [],
    "texts": [
      "\n[Life scan result]: no traces of life could be found.",
      "\n[Life scan result]: no life could be found.",
      "\n[Life scan result]: traces life could be found, but not enough to give a definitive answer.",
      "\n[Life scan result]: single-cellular organisms (no DNA).",
      "\n[Life scan result]: single-cellular organisms (complex DNA).",
      "\n[Life scan result]: multi-cellular organisms (no DNA, not sentient).",
      "\n[Life scan result]: multi-cellular organisms (complex DNA, not sentient).",
      "\n[Life scan result]: simple plant life.",
      "\n[Life scan result]: complex plant life, but no animals or bacteria",
      "\n[Life scan result]: Primitive, sentient life forms detected"
    ]
  },
  {
    "key": "planetCuddlyCreatures",
    "rarity": 5,
    "requiredTypes": [
      "planet"
    ],
    "randomizedValue": [
      {
        "key": "deadSince",
        "min": 10,
        "max": 100
      }
    ],
    "randomizedName": [
      {
        "key": "creatures",
        "type": "cuddly"
      },
      {
        "key": "enemies",
        "type": "dark_person"
      },
      {
        "key": "practice1",
        "type": "cuddly_verb"
      },
      {
        "key": "practice2",
        "type": "cuddly_verb"
      },
      {
        "key": "practice3",
        "type": "cuddly_verb"
      }
    ],
    "texts": [
      "%planet now looks like an uninhabited and barren rock, but this was not always the case. About %deadSince millenia ago, a race of creatures known as %creatures swarmed the planets surface. They were extremely social and peaceful - so much in fact, that they are coined with the invention of %practice1ing and %practice2ing (which sounds absurd, now that everybody does it). But when a large armada of %enemies battleships starts to bombard your planet, you can not just %practice3 your way out of that..."
    ]
  },
  {
    "key": "revenge",
    "requiredTypes": ["agent"],
    "texts": [
      "The betrayal felt by %agent drives him to seek revenge against all that wronged him.",
      "%agent felt betrayed. He is acting out of pure hatred and seeks revenge at all costs."
    ]
  }
]
