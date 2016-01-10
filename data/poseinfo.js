{
	"comments": [
		"RLBN -- Right, Left, Both, Neither",
		"R:1, L:2, B:4, N:8",
		"RLB: 7",
		"RL: 3",
		
		"Pose types ---",
		"0: human",
		"1: weapon",
		
		"E_PIK_POSETYPE corresponds to E_ENTTYPE"
		
	],
	
	
	"templates" : {
		"default":{
			"E_PIK_POSETYPE":0,
			"E_PIK_SUBTYPE":"E_SUB_DEFAULT",
			"E_PIK_NUMSTEPS":1,
			"E_PIK_EXTRASTEPS":0,
			"E_PIK_RLBN":8,
			"E_PIK_DOLOOP":0,
			"E_PIK_LERPSPEED":0.005,
			"E_PIK_TIMEINTERVAL":1.5
		},
		"attackWeapon":{
			"E_PIK_POSETYPE":0,
			"E_PIK_SUBTYPE":"E_SUB_SWING",
			"E_PIK_NUMSTEPS":3,
			"E_PIK_EXTRASTEPS":1,
			"E_PIK_RLBN":7,
			"E_PIK_DOLOOP":0,
			"E_PIK_LERPSPEED":0.01,
			"E_PIK_TIMEINTERVAL":0.25
		},
		"attackPunch":{
			"E_PIK_POSETYPE":0,
			"E_PIK_SUBTYPE":"E_SUB_PUNCH",
			"E_PIK_NUMSTEPS":3,
			"E_PIK_EXTRASTEPS":1,
			"E_PIK_RLBN":3,
			"E_PIK_DOLOOP":0,
			"E_PIK_LERPSPEED":0.02,
			"E_PIK_TIMEINTERVAL":0.2
		},
		"attackKick":{
			"E_PIK_POSETYPE":0,
			"E_PIK_SUBTYPE":"E_SUB_KICK",
			"E_PIK_NUMSTEPS":3,
			"E_PIK_EXTRASTEPS":1,
			"E_PIK_RLBN":3,
			"E_PIK_DOLOOP":0,
			"E_PIK_LERPSPEED":0.005,
			"E_PIK_TIMEINTERVAL":0.25
		},
		"weapon":{
			"E_PIK_POSETYPE":1,
			"E_PIK_SUBTYPE":"E_SUB_WEAPON",
			"E_PIK_NUMSTEPS":1,
			"E_PIK_EXTRASTEPS":0,
			"E_PIK_RLBN":8,
			"E_PIK_DOLOOP":0,
			"E_PIK_LERPSPEED":1.0,
			"E_PIK_TIMEINTERVAL":0.1
		}
	},
	
	"poses" : {
		
		"E_PG_TPOSE":{
			"template":"default"
		},
		"E_PG_NONPOSE":{
			"template":"default"
		},
		"E_PG_JUMP":{
			"template":"default"
		},
		"E_PG_DEAD":{
			"template":"default"
		},
		"E_PG_PICKUP":{
			"template":"default",
			"E_PIK_TIMEINTERVAL":0.25
		},
		"E_PG_IDLE":{
			"template":"default",
			"E_PIK_DOLOOP":1,
			"E_PIK_LERPSPEED":0.003,
			"E_PIK_TIMEINTERVAL":1.0,
			"E_PIK_NUMSTEPS":2
			
		},
		"E_PG_WALKFORWARD":{
			"template":"default",
			"E_PIK_DOLOOP":1,
			"E_PIK_TIMEINTERVAL":0.5,
			"E_PIK_NUMSTEPS":4
		},
		
		
		"E_PG_SLSH":{
			"template":"attackWeapon"
		},
		"E_PG_BACK":{
			"template":"attackWeapon"
		},
		"E_PG_HACK":{
			"template":"attackWeapon"
		},
		"E_PG_STAB":{
			"template":"attackWeapon"
		},
		
		
		
		
		"E_PG_HOOK":{
			"template":"attackPunch"
		},
		"E_PG_ELBO":{
			"template":"attackPunch"
		},
		"E_PG_UPPR":{
			"template":"attackPunch"
		},
		"E_PG_JABP":{
			"template":"attackPunch"
		},
		
		
		
		"E_PG_ROUN":{
			"template":"attackKick"
		},
		"E_PG_REVR":{
			"template":"attackKick"
		},
		"E_PG_BKIK":{
			"template":"attackKick"
		},
		"E_PG_FRNT":{
			"template":"attackKick",
			"E_PIK_LERPSPEED":0.02,
			"E_PIK_TIMEINTERVAL":0.2
		},
		
		
		"E_PG_WPSWORD":{
			"template":"weapon"
		},
		
		"E_PG_WPAXE":{
			"template":"weapon"
		},
		
		"E_PG_WPMACE":{
			"template":"weapon"
		},
		
		"E_PG_WPHAMMER":{
			"template":"weapon"
		},
		
		"E_PG_WPSTAFF":{
			"template":"weapon"
		},
		
		"E_PG_WPSPEAR":{
			"template":"weapon"
		},
		
		"E_PG_WPTPOSE":{
			"template":"weapon"
		}
		
	}
}
