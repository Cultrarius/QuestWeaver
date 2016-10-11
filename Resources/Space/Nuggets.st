// allowed "requiredTypes":
// artifact, deadCivilization, planet, solarSystem, agent, location, spaceShip, spaceStation, spaceWreck

// possible "randomizedName" types:
// dark_thing, dark_person, alien, light_thing, light_person, corporation, funny, ship, cuddly, cuddly_verb, venerable

// the "rarity" is just a number > 1 and the bigger the number, the rarer the nugget.
[
  {
    "key": "empty",
    "requiredTypes": [],
    "texts": [
      ""
    ]
  },
  {
    "key": "actorIntroMysterious",
    "requiredTypes": [
      "agent"
    ],
    "randomizedValue": [
      {
        "key": "serial1",
        "min": 1000,
        "max": 9999
      },
      {
        "key": "serial2",
        "min": 1000,
        "max": 9999
      },
      {
        "key": "serial3",
        "min": 1000,
        "max": 9999
      }
    ],
    "randomizedName": [
      {
        "key": "speciesName",
        "type": "alien"
      },
      {
        "key": "company",
        "type": "corporation"
      }
    ],
    "texts": [
      "%agent is a mysterious being roaming the galaxy in search of a lost soul.",
      "%agent is an unidentified stranger whose planet of origin is unknown. He is believed to belong to the species %speciesName, but no one has yet been able to confirm this.",
      "%agent is an unidentified stranger whose planet of origin as well as gender is unknown. He is believed to belong to the species %speciesName, but no one has yet been able to confirm this. The only thing known about him at this moment is his fable for trenchcoats and fedoras.",
      "%agent is an unidentified stranger whose planet of origin as well as gender is unknown. He is believed to belong to the species %speciesName, but no one has yet been able to confirm this. The only thing known about him at this moment is that he is never seen without his kitty lighter.",
      "%agent is an unidentified stranger whose planet of origin as well as gender is unknown. He is believed to belong to the species %speciesName, but no one has yet been able to confirm this. It has however been confirmed that his hair is not natural.",
      "%agent is an unidentified being roaming the galaxy. It is speculated that he might in fact be several being all sharing the same name, which might explain his ability to be in several places at once.",
      "%agent is an unidentified being roaming the galaxy. It is speculated that he might in fact be several being all sharing the same name, which might explain his ability to be in several places at once - as well as his constantly changing looks.",
      "After the galaxy-wide ban of uncontrolled artificial emotions, %agent (better known under his serial number %serial1-%serial2-%serial3) went off the grid and is trying to evade his destruction by %company ever since.",
      "Ever since %agent lost a digital knife fight, he made good on his promise never to show his face again. The fact that he ran out of acne cream might also have something to do with it."
    ]
  },
  {
    "key": "actorIntroFriendly",
    "randomizedName": [
      {
        "key": "alien",
        "type": "alien"
      },
      {
        "key": "tyrantName",
        "type": "dark_person"
      }
    ],
    "requiredTypes": [
      "agent"
    ],
    "texts": [
      "%agent is the alien overlord of the %alien species. He rose to power after curageously overthrowing the tyrant %tyrantName under whose oppression the %alien slaved for more than 300 galactic years.",
      "%agent has four legs, a long tail and is considered superbly fluffy. He shows his good intentions by wagging said tail and emitting high yelping noises. Seemingly subordinant to the dominant species on his home planet, he and the other member of his species in fact control them; they do this mainly by being irresistibly cute and thus gain them to fulfill their every wish.",
      "%agent has four legs, a long tail and is considered superbly fluffy. He shows his good intentions by wagging said tail and emitting high yelping noises. Sadly, his species is now almost extinct since the sounds are considered to be an exquisit delicacy by sound-hunters across the galaxy.",
      "%agent is a famed transgender opera singer on her home planet. The beginning lines of her most famous song are: 'Rrŕrr iiIii hôlo úyUyUy TafOsAmAmRuoY Rrŕriii iyIyIy??!!??'.",
      "%agent is an eight year old boy with a propensity for battling vorticons, rescuing bearded men and saving his babysitter from aliens.",
      "%agent is a star poet of universal fame. The poem that catapulted him to stardom "
    ]
  },
  {
    "key": "agentAge",
    "requiredTypes": [
      "agent"
    ],
    "randomizedName": [
      {
        "key": "senileHome",
        "type": "funny"
      }
    ],
    "texts": [
      "Even though the average lifespan of his species is extensive, %agent has surpassed all expectations regarding his age. What an accompilshment.",
      "Even though the average lifespan of his species is extensive, %agent has surpassed all expectations regarding his age. As he outlived everybody he has ever known, nobody shows up for his birthday party any more.",
      "Even though the average lifespan of his species is extensive, %agent has surpassed all expectations regarding his age. This is annoying for both his heirs and his caretakers at the %senileHome old folks' home.",
      "On his homeplanet, %agent is considered remarkably old. Words used to describe him include 'geriatric', 'decrepit', 'fossilized' and 'not young'.",
      "On his homeplanet, %agent is considered remarkably young. Words used to describe him include 'newborn', 'unfinished', 'infantile' and 'not old'.",
      "%agent's age is entirely unremarkable, which in itself is remarkable.",
      "%agent is the youngest ever to hold his office. Words used to describe him include 'newborn', 'unfinished', 'infantile' and 'not old'.",
      "%agent is the youngest ever to hold his office.",
      "%agent is the youngest ever to hold his office. Due to his enormous intellect and shoe size, he was promoted here at the age of six.",
      "%agent is the youngest ever to hold his office, which had to be created entirely for him after he had completed all the existing ones at the age of four. This might be what led him to wear a cape inscribed 'born ready' at all times."
    ]
  },
  {
    "key": "agentHobby",
    "requiredTypes": [
      "agent"
    ],
    "randomizedName": [
      {
        "key": "senileHome",
        "type": "funny"
      },
      {
        "key": "practice1",
        "type": "cuddly_verb"
      },
      {
        "key": "planet",
        "type": "light_thing"
      }
    ],
    "texts": [
      "%agent's hobbies include louse-back riding, intergalactic chess and rubbing fresh towels over his face.",
      "He likes to %practice1 during long and uneventful evenings. Although %practice1ing was banned on his planet, %agent is considered an expert at this fine art.",
      "%agent's hobbies include always carrying a towel and not panicking.",
      "%agent enjoys watching the sunrise. This is particularly sad as the sun is never visible from his home planet.",
      "Ever since %agent witnessed an accident at the local nuclear science lab, he has an otherwise inexplicable fear of mutants.",
      "%agent enjoys long walks on the beach, honey-and-vanilla ice cream pie and declaring war on other species.",
      "The only thing %agent loves more than doing cross-fit is telling people about it.",
      "Having studies classical philology, %agent is now a self-employed barrista on the street corners of planet %planet.",
      "Career-driven and confident, %agent never fails to send you his electronic buisness card while vigorously yelling his own name.",
      "%agent has a long beard and always wears a white robe. He considers himself the messiah of his people; however, others seldom share this view, which has resulted in him being covered in canned smaghoggi on more than one occasion.",
      "Ever since planet %planet stopped receiving radio signals, %agent has been forced to reenact all the roles of his favorite TV show by himself. The silver lining is that he never has an audience."
    ]
  },
  {
    "key": "actorIntroMachine",
    "requiredTypes": [
      "agent"
    ],
    "texts": [
      "%agent appears to be a sentient machine, which outlived its creators.",
      "%agent appears to be a sentient machine, but the creator is unknown."
    ]
  },
  {
    "key": "actorUnknownOrigin",
    "requiredTypes": [
      "agent"
    ],
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
      "[End of datalog entry %entryNumber]",
      "[Excerpt from datalog entry %entryNumber]",
      "[Datalog entry %entryNumber]",
      "[Warning: datalog entry corrupted]"
    ]
  },
  {
    "key": "locationStrangeGravity",
    "rarity": 5,
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
      "[Metal scan result]: no significant amount of valuable metals in the planet's crust.",
      "[Metal scan result]: no amount of valuable metals in the planet's crust.",
      "[Metal scan result]: no valuable metals in the planet's crust.",
      "[Metal scan result]: no metal at all, since the planet is a gas giant.",
      "[Metal scan result]: High amount of Hg (%percent1%) and Ur salts (%percent2%).",
      "[Metal scan result]: High amount of raw, liquid Hg (%percent1%).",
      "[Metal scan result]: High amount of Al (%percent1%) and Mo (%percent2%).",
      "[Metal scan result]: High amount of Ge (%percent1%) and Ur (%percent2%)."
    ]
  },
  {
    "key": "planetLife",
    "requiredTypes": [],
    "texts": [
      "[Life scan result]: no traces of life could be found.",
      "[Life scan result]: no life could be found.",
      "[Life scan result]: traces life could be found, but not enough to give a definitive answer.",
      "[Life scan result]: single-cellular organisms (no DNA).",
      "[Life scan result]: single-cellular organisms (complex DNA).",
      "[Life scan result]: multi-cellular organisms (no DNA, not sentient).",
      "[Life scan result]: multi-cellular organisms (complex DNA, not sentient).",
      "[Life scan result]: simple plant life.",
      "[Life scan result]: complex plant life, but no animals or bacteria",
      "[Life scan result]: Primitive, sentient life forms detected"
    ]
  },
  {
    "key": "planetCuddlyCreatures",
    "rarity": 8,
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
    "requiredTypes": [
      "agent"
    ],
    "texts": [
      "The betrayal felt by %agent drives him to seek revenge against all that wronged him.",
      "%agent felt betrayed. He is acting out of pure hatred and seeks revenge at all costs."
    ]
  },
  {
    "key": "shipOriginOwner",
    "requiredTypes": [
      "spaceShip",
      "agent"
    ],
    "randomizedName": [
      {
        "key": "origin",
        "type": "dark_thing"
      }
    ],
    "texts": [
      "The %spaceShip first saw the cold starlight in the %origin star system. It was later stolen by %agent, who then mainly used it to ambush trade vessels."
    ]
  }
]
