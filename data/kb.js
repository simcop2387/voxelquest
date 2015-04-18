{
  
  "comments":[
    "First letter denotes type of vocabulary:",
    "n - noun or terminal",
    "v - variable, can bind with anything",
    "p - predicate or functor, used to describe relations between words",
    "o - operator, special symbol used for specific function",
    "In vocabulary, number that follows is the number of arguments",
    "If number is -1, it accepts 2 to 2+x arguments"
  ],
  
  "vocabulary":[
    
    ["fFloat",0],
    
    ["oIfThen",2],
    ["oAll",-1],
    ["oAny",-1],
    ["oNone",-1],
    ["oList",-1],
    ["oEqual",2],
    ["oNotEqual",2],
    ["oIsBetween",3],
    
    
    ["vX",0],
    ["vY",0],
    ["vZ",0],
    
    
    
    ["nBob",0],
    ["nSue",0],
    ["nSteve",0],
    ["nItem0",0],
    ["nItem1",0],
    
    ["pIsClass",2],
    ["pIsJealous",3],
    ["pLoves",2],
    
    ["pLimbsRequired",2],
    ["pIsHeld",1],
    
    ["pIsIronSword",1],
    ["pIsClothBag",1],
    ["pIsDoombringer",1],
    ["pIsArtifact",1],
    ["pIsContainer",1],
    
    ["pIsMetal",1],
    ["pIsPreciousMetal",1],
    ["pIsStone",1],
    ["pIsWood",1],
    ["pIsCloth",1],
    ["pIsHide",1],
    ["pIsGemstone",1],
    
    ["pIsCopper",1],
    ["pIsSilver",1],
    ["pIsGold",1],
    ["pIsPlatinum",1],
    
    ["pIsOnyx",1],
    ["pIsAmethyst",1],
    ["pIsTopaz",1],
    ["pIsSapphire",1],
    ["pIsEmerald",1],
    ["pIsRuby",1],
    ["pIsDiamond",1],
    
    ["pIsSandstone",1],
    ["pIsMarble",1],
    ["pIsGranite",1],
    
    ["pIsBronze",1],
    ["pIsIron",1],
    ["pIsSteel",1],
    
    ["pIsOak",1],
    ["pIsRedwood",1],
    ["pIsYew",1],
    
    ["pIsCotton",1],
    ["pIsLinen",1],
    ["pIsSilk",1],
    
    ["pIsLeather",1],
    ["pIsDragonScale",1],
    
    
    ["pIsEdible",1],
    ["pIsEquipable",1],
    ["pIsTool",1],
    ["pIsWeapon",1],
    
    ["pIsSword",1],
    ["pIsCloak",1],
    ["pIsRing",1],
    ["pIsArmor",1],
    ["pIsShield",1],
    ["pIsBag",1],
    
    
    ["pIsHumanoid",1],
    
    ["pHasHead",2],
    ["pHasNeck",2],
    ["pHasBack",2],
    ["pHasTorso",2],
    ["pHasLegs",2],
    ["pHasFeet",2],
    ["pHasArms",2],
    ["pHasHands",2],
    ["pHasFingers",2],
    
    
    ["pCoversHead",1],
    ["pCoversNeck",1],
    ["pCoversBack",1],
    ["pCoversTorso",1],
    ["pCoversLegs",1],
    ["pCoversFeet",1],
    ["pCoversArms",1],
    ["pCoversHands",1],
    ["pCoversFingers",1]
    
    
    
    
  ],
  
  "kb":[
  
  
    
    "oIfThen",[
      "pIsPreciousMetal",["vX"],
      "pIsMetal",["vX"]
    ],
    
    "oIfThen",[
      "oAny",[
        "pIsCopper",["vX"],
        "pIsSilver",["vX"],
        "pIsGold",["vX"],
        "pIsPlatinum",["vX"]
      ],
      "pIsPreciousMetal",["vX"]
    ],
    
    "oIfThen",[
      "oAny",[
        "pIsBronze",["vX"],
        "pIsIron",["vX"],
        "pIsSteel",["vX"]
      ],
      "pIsMetal",["vX"]
    ],
    
    
    "oIfThen",[
      "oAny",[
        "pIsOak",["vX"],
        "pIsRedwood",["vX"],
        "pIsYew",["vX"]
      ],
      "pIsWood",["vX"]
    ],
    
    "oIfThen",[
      "oAny",[
        "pIsSandstone",["vX"],
        "pIsMarble",["vX"],
        "pIsGranite",["vX"]
      ],
      "pIsStone",["vX"]
    ],
    
    "oIfThen",[
      "oAny",[
        "pIsOnyx",["vX"],
        "pIsAmethyst",["vX"],
        "pIsTopaz",["vX"],
        "pIsSapphire",["vX"],
        "pIsEmerald",["vX"],
        "pIsRuby",["vX"],
        "pIsDiamond",["vX"]
      ],
      "pIsGemstone",["vX"]
    ],
    
    
    "oIfThen",[
      "oAny",[
        "pIsCotton",["vX"],
        "pIsLinen",["vX"],
        "pIsSilk",["vX"]
      ],
      "pIsCloth",["vX"]
    ],
    
    "oIfThen",[
      "oAny",[
        "pIsLeather",["vX"],
        "pIsDragonScale",["vX"]
      ],
      "pIsHide",["vX"]
    ],
    
    "oIfThen",[
      "pIsSword",["vX"],
      "pIsWeapon",["vX"]
    ],
    
    "oIfThen",[
      "pIsSword",["vX"],
      "pLimbsRequired",["vX",1]
    ],
    
    "oIfThen",[
      "pIsBag",["vX"],
      "pIsContainer",["vX"]
    ],
    
    
    "oIfThen",[
      "pIsDoombringer",["vX"],
      "oList",[
        "pIsSword",["vX"],
        "pIsSteel",["vX"],
        "pIsArtifact",["vX"]
      ]
    ],
    
    "oIfThen",[
      "pIsIronSword",["vX"],
      "oList",[
        "pIsSword",["vX"],
        "pIsIron",["vX"]
      ]
    ],
    
    "oIfThen",[
      "pIsClothBag",["vX"],
      "oList",[
        "pIsCloth",["vX"],
        "pIsBag",["vX"]
      ]
    ],
    
    
    "pIsIronSword",["nItem0"],
    "pIsIronSword",["nItem1"],
    
    
    
    "oIfThen",[
      "oAll",[
        "pLoves",["vX","vZ"],
        "pLoves",["vZ","vY"],
        "oNotEqual",["vX","vY"]
      ],
      "pIsJealous",["vX","vY"]
    ],
    
    "pLoves",["nSteve","nSue"],
    "pLoves",["nBob","nSue"],
    "pLoves",["nSue","nSteve"],
    
    
    
    "oIfThen",[
      "oAny",[
        "pCoversHead",["vX"],
        "pCoversNeck",["vX"],
        "pCoversBack",["vX"],
        "pCoversTorso",["vX"],
        "pCoversLegs",["vX"],
        "pCoversFeet",["vX"],
        "pCoversArms",["vX"],
        "pCoversHands",["vX"],
        "pCoversFingers",["vX"],
        "pIsHeld",["vX"]
      ],
      "pIsEquipable",["vX"]
    ],
    
    "oIfThen",[
      "pIsHumanoid",["vX"],
      "oList",[
        "pHasHead",["vX", 1],
        "pHasNeck",["vX", 1],
        "pHasBack",["vX", 1],
        "pHasTorso",["vX", 1],
        "pHasLegs",["vX", 2],
        "pHasFeet",["vX", 2],
        "pHasArms",["vX", 2],
        "pHasHands",["vX", 2],
        "pHasFingers",["vX", 2]
      ]
    ],
    
    
    
    "oIfThen",[
      "pWantsToStayAlive",["vX"],
      "oList",[
        "pWantsToSatiateHunger",["vX"],
        "pWantsToQuenchThirst",["vX"],
        "pWantsToPreventDamage",["vX"]
      ]
      
    ],
    
    
    
    "oIfThen",[
      "oAll",[
        "pIsClass",["vX","vY"],
        "oIsBetween",["vY",0,71]
      ],
      "pIsRing",["vX"]
    ],
    
    "oIfThen",[
      "oAll",[
        "pIsClass",["vX","vY"],
        "oIsBetween",["vY",72,131]
      ],
      "pIsCloak",["vX"]
    ],
    
    "oIfThen",[
      "oAll",[
        "pIsClass",["vX","vY"],
        "oIsBetween",["vY",132,155]
      ],
      "pIsArmor",["vX"]
    ],
    
    "oIfThen",[
      "oAll",[
        "pIsClass",["vX","vY"],
        "oIsBetween",["vY",156,179]
      ],
      "pIsShield",["vX"]
    ],
    
    "pIsClass",["nItem0",162]
    
    
    
    
    
    
    
  ],
  
  "qb":[
    "pIsShield",["vX"]
  ]
  
  
}
