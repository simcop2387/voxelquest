{
	"templates": {
		"cont": {
			"type":0
		},
		"tooltip": {
			"type":0,
			"hoverType":1,
			"layer":3,
			"hasBackground":0
		},
		"tooltipValue": {
			"type":0,
			"hoverType":3,
			"layer":3,
			"hasBackground":0
		},
		"popout": {
			"type":0,
			"hoverType":2,
			"layer":1,
			"hasBackground":0
		},
		"toggle": {
			"type":1,
			"divisions":1
		},
		"slider": {
			"type":1
		},
		"button": {
			"type":2
		},
		"radio": {
			"type":3
		}
	},
	
	"sounds": {
		"mouseDown":{"name":"castinet0","vol":0.5},
		"mouseUp":{"name":"castinet0","vol":1.0},
		"mouseOver":{"name":"claves0","vol":0.6},
		"mouseOut":{"name":"","vol":1.0},
		"showGUI":{"name":"woodblock0","vol":1.0},
		"hideGUI":{"name":"woodblock0","vol":1.0}
	},
	
	
	"comment_1":"Max of 256 materials",
	
	"materialDefs": {
		"none":{"class":"none"},
	
		"Copper":{"class":"Ornate"},
		"Silver":{"class":"Ornate"},
		"Gold":{"class":"Ornate"},
		"Platinum":{"class":"Ornate"},
		
		
		"Onyx":{"class":"Gemstone"},
		"Amethyst":{"class":"Gemstone"},
		"Topaz":{"class":"Gemstone"},
		"Sapphire":{"class":"Gemstone"},
		"Emerald":{"class":"Gemstone"},
		"Ruby":{"class":"Gemstone"},
		"Diamond":{"class":"Gemstone"},
		
		"Sandstone":{"class":"Stone"},
		"Marble":{"class":"Stone"},
		"Granite":{"class":"Stone"},
	
		"Bronze":{"class":"Metal"},
		"Iron":{"class":"Metal"},
		"Steel":{"class":"Metal"},
		
		"Oak":{"class":"Wood"},
		"Redwood":{"class":"Wood"},
		"Yew":{"class":"Wood"},
		
		"Cotton":{"class":"Cloth"},
		"Linen":{"class":"Cloth"},
		"Silk":{"class":"Cloth"},
		
		"Leather":{"class":"Hide"},
		"Dragon Scale":{"class":"Hide"},
		"Yew":{"class":"Hide"}
		
	},
	
	"itemDefs": {
		"none":{
			"iconNum":1367,
			"class":"none"
		},
		"Helm":{
			"iconNum":271,
			"class":"Helmet"
		},
		"Pendant":{
			"iconNum":20,
			"class":"Necklace"
		},
		"Cloak":{
			"iconNum":54,
			"class":"Cloak"
		},
		"Plated Mail":{
			"iconNum":126,
			"class":"Armor"
		},
		"Ring":{
			"iconNum":933,
			"class":"Gloves"
		},
		"Long Sword":{
			"iconNum":1158,
			"class":"Primary"
		},
		"Kite Shield":{
			"iconNum":975,
			"class":"Secondary"
		},
		"Boots":{
			"iconNum":130,
			"class":"Boots"
		}
	},
	
	"inventoryData":{
		"items":[
			{"eq":1, "quant":"1", "name":"Helm", "mat":"Iron"},
			{"eq":1, "quant":"1", "name":"Pendant", "mat":"Ruby"},
			{"eq":1, "quant":"1", "name":"Cloak", "mat":"Silk"},
			{"eq":1, "quant":"1", "name":"Plated Mail", "mat":"Iron"},
			{"eq":1, "quant":"1", "name":"Ring", "mat":"Ruby"},
			{"eq":1, "quant":"1", "name":"Long Sword", "mat":"Iron"},
			{"eq":1, "quant":"1", "name":"Kite Shield", "mat":"Iron"},
			{"eq":1, "quant":"1", "name":"Boots", "mat":"Leather"}
		]
	},
	
	
	
	
	
	"curMenu":"shaderParamMenu",
	
	
	
	
	
	"shaderParamMenu": {
		"template":"cont",
		"fillRatioX":1,
		"children": [
			{
				"template":"cont",
				"fillDir":1,
				"isInternal":1,
				"dataFile":"shaderParams",
				"dataSource":"params",
				"childType":"shaderParams",
				"children": [],
				"childTemplate":{
					"fillRatioX":1,
					"template":"slider",
					"label":"",
					
					"callback":"updateShaderParam",
					"callbackData":{
						"shaderName":"",
						"paramName":""
					},
					
					"floatingChildren":[
						{
							"template":"tooltipValue",
							"children":[
								{
									"template":"cont",
									"label":"",
									"ss":"tooltipSS"
								}
							]
						}
					]
				}
			}
		]
	},
	
	"inventoryMenu": {
		"template":"cont",
		"fillRatioX":1,
		"children": [
			{
				"template":"cont",
				"fillDir":1,
				"children": [
				
					{
						"template":"cont",
						"label":"Equipped",
						"alignX":1,
						"fillRatioX":1,
						"ss":"headerSS"
						
					},
					{
						"template":"cont",
						"fillDir":1,
						"dataFile":"",
						"dataSource":"inventoryData.items",
						"whereAllEqual":[
							["eq",1]
						],
						"whereAnyEqual":[
						
						],
						"childType":"inventoryItem",
						"childTemplate":{
							"fillRatioX":1,
							"template":"radio",
							"label":"",
							"floatingChildren":[
								{
									"template":"tooltip",
									"children":[
										{
											"template":"cont",
											"label":"",
											"ss":"tooltipSS"
										}
									]
								}
							]
														
						},
						"children": []
					}
				
				]
			}
		]
	},	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	"testMenu2": {
		"template":"cont",
		"fillRatioX":1,
		"children": [
			
			{
				"template":"cont",
				"fillDir":1,
				"children": [
					{
						"template":"radio",
						"label":"Lorem 32& 45& 74& 22& 84& 66& ipsum dolor sit amet 0",
						"floatingChildren":[
							{
								"template":"tooltip",
								"children":[
									{
										"template":"cont",
										"label":"I am a tooltip",
										"uid":"testTT",
										"ss":"tooltipSS"
									}
								]
							}
						]
						
					},
					{
						"template":"radio",
						"label":"Lorem ipsum dolor 120& 37& sit amet 1",
						
						"floatingChildren":[
							{
								"template":"popout",
								
								"children":[
									{
										"template":"cont",
										"fillDir":1,
										"children":[
											
											{
												"template":"radio",
												"label":"Lorem 32& 45& 74& 22& 84& 66& ipsum dolor sit amet 0"
												
											},
											{
												"template":"radio",
												"label":"Lorem ipsum dolor 120& 37& sit amet 1"
																						
											},
											{
												"template":"radio",
												"label":"44& Lorem ipsum dolor sit amet 2"
											}
											
										]
									}
								]
								
								
							}
						]
						
					},
					{
						"template":"radio",
						"label":"44& Lorem ipsum dolor sit amet 2"
					}
				]
			}
		]
	}
	
}

