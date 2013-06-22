
var TRACE_ON = true;
var gob;

function g_trim(str) {
    return str.replace(/^\s\s*/, '').replace(/\s\s*$/, '');
}

if(!Array.prototype.last) {
    Array.prototype.last = function() {
        return this[this.length - 1];
    }
}

$(function() {

	gob = {
		traceLevel: 0,
		hasConnection:false,
		popCount: 0,
		traceArr: [],
		lastMessage: null,
		maxFrameSize: 16777216,
		pushTrace: function() {
			gob.traceLevel++;
			gob.popCount = 0;
		},
		popTrace: function() {
			

			gob.traceLevel--;
			if (gob.traceLevel < 0) {
				gob.traceLevel = 0;
			}

			if (gob.traceLevel == 0 && gob.popCount >= 1) {
				gob.doTrace("END");
			}
			else {
				if (gob.popCount >= 2) {
					gob.doTraceTab("END");
				}

				
			}
			
		},
		doTrace:function(traceStr, otherPrefix) {
			
			if (TRACE_ON) {
				gob.popCount++;

				var op = otherPrefix;

				if (op) {} else {
					op = "";
				}

				var prefix = gob.getTracePrefix() + op;
				var prefixn = "\n"+prefix;

				if (typeof traceStr == "object") {

					var objStr = JSON.stringify(traceStr,null,"\t");
					var newStr = prefix + objStr.replace(/[\r\n]/g, prefixn);

					console.log( newStr );
				}
				else {
					console.log(prefix + traceStr);
				}
			}
			else {
				console.log(traceStr);
			}
			
		},
		doTraceTab: function(traceStr) {
			if (TRACE_ON) {
				gob.doTrace(traceStr,"|  ");
			}
			else {
				console.log(traceStr);
			}
			
		},
		getTracePrefix: function() {
			return gob.traceArr[gob.traceLevel];
		}

	};

	var wf = function(varName, func) {
		if (TRACE_ON) {
			gob[varName] = function() {

				var argArr = Array.prototype.slice.call(arguments, 0);

				gob.pushTrace();
				gob.doTrace(varName + "("+argArr.join(",")+")");
				
				func.apply(func,arguments);
				gob.popTrace();
			}
		}
		else {
			gob[varName] = func;
		}

		return gob[varName];
		
	}


	gob.init = function() {
		var i;
		var curStr = "";

		gob.traceLevel = 0;
		gob.popCount = 0;
		gob.traceArr = ["",""];

		for (i = 2; i < 100; i++) {
			curStr += "|  ";
			gob.traceArr.push(curStr);
		}

		if (window.WebSocket || window.MozWebSocket) {} else {
			console.log("Browser does not support web sockets");
			return;
		}


		wf("connectionOnOpen",function() {
			gob.hasConnection = true;

			if (gob.lastMessage) {
				gob.sendMessage(gob.lastMessage);
			}

		});
		wf("connectionOnError",function(error) {
			gob.doTraceTab("ERROR: " + error);
		});
		wf("connectionOnMessage",function(message) {

			var result = JSON.parse(message.data);
			gob.doTraceTab("MESSAGE: " + message.data);

			//console.log("RESULT Y: " + result.y);
			//var json = JSON.parse(message.data);
		});
		wf("connectionClose",function() {
			gob.hasConnection = false;
		});

		wf("openNewConnection", function(url) {

			gob.connection = new WebSocket(url);
			gob.connection.onopen = gob.connectionOnOpen;
			gob.connection.onerror = gob.connectionOnError;
			gob.connection.onmessage = gob.connectionOnMessage;
			gob.connection.onclose = gob.connectionClose;

		});

		wf("sendMessage",function(msg) {

			var url = 'ws://127.0.0.1:9980';
			var sendStr;

			if (gob.hasConnection) {

				gob.doTraceTab("READYSTATE: " + gob.connection.readyState);

				if (gob.connection.readyState !== 1) {
					gob.connection.close();
					gob.lastMessage = msg;
					gob.openNewConnection(url);
				}
				else {

					sendStr = JSON.stringify(msg);
					
					if (sendStr.length + 256 >= gob.maxFrameSize) {
						doTraceTab("ERROR: Frame Size Exceeded");
					}
					else {
						gob.connection.send(sendStr);
					}

					
					gob.lastMessage = null;
				}
			}
			else {
				gob.lastMessage = msg;
				gob.openNewConnection(url);
			}
			
			
		});

		window.onbeforeunload = function() {
		    gob.connection.onclose = function () {}; // disable onclose handler first
		    gob.connection.close()
		};

	}


	//gob.init();




	if ( ! Detector.webgl ) Detector.addGetWebGLMessage();

	// globals

	var container, stats;
	var camera, scene, renderer;
	var mesh;
	var shaderNames = ["textShader"];
	var shaders = {};
	var materials = {};
	var texture;
	var curTime = 0;
	var geoBuffer;
	
	var drawTextArea = function(obj) {

		var str = obj.str;
		var x = obj.x;
		var y = obj.y;
		var scale = obj.scale;
		var pMaxW = obj.maxWidth;
		var pMaxH = obj.maxHeight;


		var xOff;
		var yOff;

		var maxW = pMaxW/scale;
		var maxH = pMaxH/scale;
		var i;
		var j;
		var k;
		var lineArr = str.split('\n');
		var wordArr = [];
		var wordObjArr = [];
		var finalLineArr = [];
		
		var curFont = g_fonts["arial_black_regular_48.png"];
		var curChar;
		var charCode;
		var curWordObj;
		var curWordWidth;
		var curWordStr;
		var curLineWidth;
		var nextCharCode;
		var resSourceX = curFont.texture.width;
		var resSourceY = curFont.texture.height;
		var curLineStr;
		var spaceWidth = curFont.chars[0].width;
		var dashWidth = curFont.chars[13].width;
		var wordMod;

		var hAlign = obj.hAlign ? obj.hAlign:0;
		var vAlign = obj.vAlign ? obj.vAlign:0;





		for (i = 0; i < lineArr.length; i++) {
			wordArr.push(lineArr[i].split(' '));
		}

		// determine width of each word
		for (i = 0; i < wordArr.length; i++) {
			wordObjArr.push([]);
			for (j = 0; j < wordArr[i].length; j++) {

				curWordWidth = 0;
				curWordStr = wordArr[i][j];

				for (k = 0; k < curWordStr.length; k++) {

					charCode = curWordStr.charCodeAt(k);
					curChar = curFont.chars[charCode-32];

					if (k < curWordStr.length - 1) {
						nextCharCode = curWordStr.charCodeAt(k+1);
						curWordWidth += curChar.width + curFont.kernMap[charCode*128 + nextCharCode];
					}
					else {
						curWordWidth += curChar.width;
					}

				}

				wordObjArr[i].push({
					wordStr:curWordStr,
					wordWidth:curWordWidth
				});
			}
		}

		// determine width of each line
		for (i = 0; i < wordObjArr.length; i++) {
			
			curLineStr = "";
			curLineWidth = 0;


			for (j = 0; j < wordObjArr[i].length; j++) {

				curWordObj = wordObjArr[i][j];

				if (curLineWidth + curWordObj.wordWidth > maxW) {
					
					
					
					finalLineArr.push({
						lineStr:curLineStr,
						lineWidth: curLineWidth
					});
					curLineStr = "";
					curLineWidth = 0;
				}
				

				if (curLineWidth == 0) {
					curLineStr += curWordObj.wordStr;
					curLineWidth += curWordObj.wordWidth;
				}
				else {
					curLineStr += " " + curWordObj.wordStr;
					curLineWidth += spaceWidth + curWordObj.wordWidth;
				}
				
			}

			finalLineArr.push({
				lineStr:curLineStr,
				lineWidth: curLineWidth
			});

		}

		// trim off newlines at end;

		while (finalLineArr.last().lineWidth == 0) {
			finalLineArr.pop();
		}


		if (obj.drawBG) {

			obj.curXB = x;
			obj.curYB = y;

			drawString(obj,true);
		}

		
		for (i = 0; i < finalLineArr.length; i++) {
			yOff = (i*curFont.metrics.ascender) + curFont.metrics.descender;
			switch (hAlign) {
				//left
				case 0:
					xOff = 0;
				break;
				// center
				case 1:
					xOff = (maxW-finalLineArr[i].lineWidth)/(2);
				break;
				// right
				case 2:
					xOff = (maxW-finalLineArr[i].lineWidth);
				break;
			}



			obj.curStr = finalLineArr[i].lineStr;
			obj.curXB = x;
			obj.curYB = y;
			obj.curXO = xOff;
			obj.curYO = yOff;
			

			if ( (i+1)*curFont.metrics.ascender < maxH) {
				drawString(obj,false);
			}


		}


		

	}

	var drawString = function(obj, isBG) {


		var str = obj.curStr;
		var xBase = obj.curXB;
		var yBase = obj.curYB;
		var xOff = obj.curXO;
		var yOff = obj.curYO;
		var scale = obj.scale;
		var isRect = obj.isRect;


		var curFont = g_fonts["arial_black_regular_48.png"];
		var curUniforms = shaders.textShader.uniforms;

		var i;
		var curChar;
		var charCode;
		var nextCharCode;

		var curX = xOff;
		var curY = yOff;
		
		var resSourceX = curFont.texture.width;
		var resSourceY = curFont.texture.height;

		var vx;
		var vy;
		var vw;
		var vh;

		var vx1;
		var vy1;
		var vx2;
		var vy2;

		var tx1;
		var ty1;
		var tx2;
		var ty2;

		var at0;
		var at1;
		var at2;
		var at3;


		var curGBLen;

		var strln = str.length;

		if (isBG) {
			strln = 1;
		}


		for (i = 0; i < strln; i++) {
			

			if (isBG) {

				vx1 = xBase;
				vy1 = yBase;
				vx2 = xBase + obj.maxWidth;
				vy2 = yBase + obj.maxHeight;

				tx1 = 0;
				ty1 = 0;
				tx2 = 0;
				ty2 = 0;

				at0 = 1.0;
				at1 = 1.0;
				at2 = 1.0;
				at3 = 1.0;

			}
			else {
				charCode = str.charCodeAt(i);
				curChar = curFont.chars[charCode-32];

				vx = (xBase + (curX + curChar.ox)*scale);
				vy = (yBase + (curY + curFont.metrics.height - (curChar.oy) )*scale);
				vw = (curChar.w)*scale;
				vh = (curChar.h)*scale;

				vx1 = vx;
				vy1 = vy;
				vx2 = vx+vw;
				vy2 = vy+vh;

				tx1 = (curChar.x)/resSourceX;
				ty1 = (resSourceY - (curChar.y) )/resSourceY;
				tx2 = (curChar.x + curChar.w)/resSourceX;
				ty2 = (resSourceY - (curChar.y + curChar.h) )/resSourceY;

				at0 = 0.0;
				at1 = 0.0;
				at2 = 0.0;
				at3 = 0.0;


				if (i < str.length - 1) {
					nextCharCode = str.charCodeAt(i+1);
					curX += curChar.width + curFont.kernMap[charCode*128 + nextCharCode];
				}
			}

			geoBuffer.vertices.push(
				new THREE.Vector3( vx1, vy1, 0 ),
				new THREE.Vector3( vx2, vy1, 0 ),
				new THREE.Vector3( vx2, vy2, 0 ),
				new THREE.Vector3( vx1, vy2, 0 )
			);

			curGBLen = geoBuffer.vertices.length - 4;

			geoBuffer.faces.push( new THREE.Face4(curGBLen+0, curGBLen+1, curGBLen+2, curGBLen+3) );

			geoBuffer.faceVertexUvs[0].push([
				new THREE.Vector2( tx1, ty1 ),
				new THREE.Vector2( tx2, ty1 ),
				new THREE.Vector2( tx2, ty2 ),
				new THREE.Vector2( tx1, ty2 )
			]);

			shaders.textShader.attributes.a_Data0.value.push(

				new THREE.Vector4( at0, at1, at2, at3 ),
				new THREE.Vector4( at0, at1, at2, at3 ),
				new THREE.Vector4( at0, at1, at2, at3 ),
				new THREE.Vector4( at0, at1, at2, at3 )

			);

		}

	}

	var onWindowResize = function( event ) {

		var i;
		for (i = 0; i < shaderNames.length; i++) {
			shaders[shaderNames[i]].uniforms.u_Resolution.value.x = window.innerWidth;
			shaders[shaderNames[i]].uniforms.u_Resolution.value.y = window.innerHeight;
		}

		renderer.setSize( window.innerWidth, window.innerHeight );


	}

	var animate = function() {

		requestAnimationFrame( animate );
		render();
		stats.update();

	}

	var render = function() {

		curTime += 0.05;
		renderer.render( scene, camera );

	}

	var init = function() {


		container = document.getElementById( 'container' );
		camera = new THREE.Camera();
		camera.position.z = 1;
		scene = new THREE.Scene();



		geoBuffer = new THREE.Geometry();
		geoBuffer.dynamic = true;





		drawTextArea({
			str:"Lorem ipsum dolor sit amet, pro graeci nostrum ullamcorper at,\n\n est meis mediocritatem eu. No ludus zril quando eum, et altera aliquam menandri per, ad his ridens discere efficiendi. An quo detracto vituperata, pro regione detracto abhorreant no, tantas sententiae te mei. Pri suavitate conclusionemque te, enim laoreet per te. Utinam aliquam detracto te sea. Lorem ipsum dolor sit amet, pro graeci nostrum ullamcorper at,\n\n est meis mediocritatem eu. No ludus zril quando eum, et altera aliquam menandri per, ad his ridens discere efficiendi. An quo detracto vituperata, pro regione detracto abhorreant no, tantas sententiae te mei. Pri suavitate conclusionemque te, enim laoreet per te. Utinam aliquam detracto te sea. Lorem ipsum dolor sit amet, pro graeci nostrum ullamcorper at,\n\n est meis mediocritatem eu. No ludus zril quando eum, et altera aliquam menandri per, ad his ridens discere efficiendi. An quo detracto vituperata, pro regione detracto abhorreant no, tantas sententiae te mei. Pri suavitate conclusionemque te, enim laoreet per te. Utinam aliquam detracto te sea.",
			x:50,
			y:100,
			scale:0.25,
			maxWidth:800,
			maxHeight:200,
			hAlign:0,
			vAlign:0,
			drawBG:true,

			curStr:"",
			curXB:0,
			curYB:0,
			curXO:0,
			curYO:0,

		});

		mesh = new THREE.Mesh( geoBuffer, materials.textShader);

		mesh.geometry.verticesNeedUpdate = true;
		mesh.geometry.elementsNeedUpdate = true;
		mesh.geometry.uvsNeedUpdate = true;
		shaders.textShader.attributes[ "a_Data0" ].needsUpdate = true;

		//mesh.geometry.morphTargetsNeedUpdate = true;
		//mesh.geometry.normalsNeedUpdate = true;
		//mesh.geometry.colorsNeedUpdate = true;
		//mesh.geometry.tangentsNeedUpdate = true;




		scene.add( mesh );
		renderer = new THREE.WebGLRenderer();
		renderer.autoClear = false;
		container.appendChild( renderer.domElement );
		stats = new Stats();
		stats.domElement.style.position = 'absolute';
		stats.domElement.style.bottom = '0px';
		container.appendChild( stats.domElement );
		onWindowResize();
		window.addEventListener( 'resize', onWindowResize, false );


	}

	var getKernMap = function(curFont) {
		var i;
		var curKern;
		var totSize = 128*128;

		curFont.kernMap = new Array(totSize);

		for (i = 0; i < totSize; i++) {
			curFont.kernMap[i] = 0;
		}

		for (i = 0; i < curFont.kernings.length; i++) {
			curKern = curFont.kernings[i];

			curFont.kernMap[ curKern.from.charCodeAt(0)*128 + curKern.to.charCodeAt(0) ] = curKern.offset;

		}

	}

	var getUniforms = function(str, curMap, varType) {
		var strArr = str.split(';\n');
		var resObj = {};
		var i;
		var uInd;
		var newStr;
		var curName;
		var strArr2;

		for (i = 0; i < strArr.length; i++) {
			uInd = strArr[i].indexOf(varType);

			if (uInd > -1) {
				newStr = strArr[i].substr(uInd);

				strArr2 = newStr.split(' ');

				if (strArr2.length == 3) {
					curType = strArr2[1];
					curName = strArr2[2];

					if (varType == "uniform") {
						switch(curType) {
							case "float":
								resObj[curName] = { type: "f", value: 0.0 };
							break;
							case "vec2":
								resObj[curName] = { type: "v2", value: new THREE.Vector2() };
							break;
							case "vec3":
								resObj[curName] = { type: "v3", value: new THREE.Vector3() };
							break;
							case "vec4":
								resObj[curName] = { type: "v4", value: new THREE.Vector4() };
							break;
							case "sampler2D":
								resObj[curName] = { type: "t", value: curMap };
							break;
							default:
								console.log("Invalid type: " + curType);
							break;
						}
					}
					else {
						switch(curType) {
							case "float":
								resObj[curName] = { type: "f", value: [] };
							break;
							case "vec2":
								resObj[curName] = { type: "v2", value: [] };
							break;
							case "vec3":
								resObj[curName] = { type: "v3", value: [] };
							break;
							case "vec4":
								resObj[curName] = { type: "v4", value: [] };
							break;
							default:
								console.log("Invalid type: " + curType);
							break;
						}
					}

					

				}
				else {
					console.log('Invalid uniform: ' + newStr);
				}
			}
		}

		//console.log(resObj);

		return resObj;
	}

	var preload = function() {

		var i;

		var myMap = THREE.ImageUtils.loadTexture( './fonts/arial_black_regular_48.png', new THREE.UVMapping(), function() { } );

		for (i = 0; i < shaderNames.length; i++) {


			jQuery.ajax({
				url: './shaders/'+shaderNames[i]+'.c',
				success: function(result) {

					var splitText = result.split("$");

					if (splitText.length != 4) {
						console.log("Invalid Shader, missing some sections ($)");
					}

					shaders[shaderNames[i]] = {};
					shaders[shaderNames[i]].map = myMap;
					shaders[shaderNames[i]].side = THREE.DoubleSide;
					shaders[shaderNames[i]].transparent = true;
					shaders[shaderNames[i]].vertexShader = splitText[0] + splitText[1] + splitText[2];
					shaders[shaderNames[i]].fragmentShader = splitText[0] + splitText[3];
					shaders[shaderNames[i]].uniforms = getUniforms(splitText[0],myMap,"uniform");
					shaders[shaderNames[i]].attributes = getUniforms(splitText[1],myMap,"attribute");

					materials[shaderNames[i]] = new THREE.ShaderMaterial(shaders[shaderNames[i]]);


				},
				async:   false,
				dataType:"text"
			});

		}

		for (i in g_fonts) {
			if (g_fonts.hasOwnProperty(i)) {
				getKernMap(g_fonts[i]);
			}
		}


		init();
		animate();
		
	}

	preload();


	





});