
var gob;

if(!Array.prototype.last) {
    Array.prototype.last = function() {
        return this[this.length - 1];
    }
}

window.performance = window.performance || {};
performance.now = (function() {
    return performance.now       ||
        performance.mozNow    ||
        performance.msNow     ||
        performance.oNow      ||
        performance.webkitNow ||
        function() {
            //Doh! Crap browser!
            return new Date().getTime(); 
        };
})();

j$(function() {


	var DisplayNode = new Class({
		initialize: function() {
			this.children = {};
			this.childArr = [];
			this.isBatchRoot = false;

		},
		addChild: function(obj) {
			this.childArr.push(obj);
			this.children[obj.nodeId] = obj;
		}
	});

	var container, stats;
	var camera, scene, renderer;
	var texture;
	var testObj;
	var zoom = 1;
	var atArr = [0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0];
	
	var rtScene;
	var layerScene;
	
	var meshText;
	var meshBG;
	var meshFSQ;
	

	gob = {
		curDebugSection:0.0,


		isRendering: true,
		maxLayers: 40,
		updateBaseRT:true,
		renderTargets:{},

		shaderNames:["reflShader","lightingShader","aoShader","layerShader","bgShader","textShader","heightShader","normShader","downscaleShader","upscaleShader","debugShader"],
		fontNames:["arial_black_regular_48","arial_black_regular_96"],
		fontLoaded:{},
		shaders:{},
		materials:{},
		debugTex:null,


		displayList:[],
		traceOn:true,
		traceLevel: 0,
		hasConnection:false,
		popCount: 0,
		traceArr: [],
		lastMessage: null,
		maxFrameSize: 16777216,
		doError: function(str) {
			console.log(str);
		},
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
			
			if (gob.traceOn) {
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
			if (gob.traceOn) {
				gob.doTrace(traceStr,"|  ");
			}
			else {
				console.log(traceStr);
			}
			
		},
		getTracePrefix: function() {
			return gob.traceArr[gob.traceLevel];
		},
		wf:function(varName, func) {
			if (gob.traceOn) {
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

	};

	gob.wf("connectionOnOpen",function() {
		gob.hasConnection = true;

		if (gob.lastMessage) {
			gob.sendMessage(gob.lastMessage);
		}

	});
	gob.wf("connectionOnError",function(error) {
		gob.doTraceTab("ERROR: " + error);
	});
	gob.wf("connectionOnMessage",function(message) {

		var result = JSON.parse(message.data);
		gob.doTraceTab("MESSAGE: " + message.data);

	});
	gob.wf("connectionClose",function() {
		gob.hasConnection = false;
	});

	gob.wf("openNewConnection", function(url) {

		gob.connection = new WebSocket(url);
		gob.connection.onopen = gob.connectionOnOpen;
		gob.connection.onerror = gob.connectionOnError;
		gob.connection.onmessage = gob.connectionOnMessage;
		gob.connection.onclose = gob.connectionClose;

	});

	gob.wf("sendMessage",function(msg) {

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

	gob.doTextUpdate = function() {
		gob.drawTextArea(testObj, true, false, meshBG);
		gob.drawTextArea(testObj, false, false, meshText);
		gob.updateBaseRT = true;
	}

	gob.initFinal = function() {
		var curShader;
		var i;

		
		// normal x
		// normal y
		// depth
		// tex id

		
		for (i = 0; i < gob.shaderNames.length; i++) {


			j$.ajax({
				url: './shaders/'+gob.shaderNames[i]+'.c',
				success: function(result) {

					var splitText = result.split("$");

					if (splitText.length != 4) {
						gob.doError("Invalid Shader, missing some sections ($)");
					}

					gob.shaders[gob.shaderNames[i]] = {};
					curShader = gob.shaders[gob.shaderNames[i]];


					switch (gob.shaderNames[i]) {
						
						case "debugShader":
							curShader.transparent = false;
						break;

						case "heightShader":
							curShader.transparent = false;
						break;

						case "normShader":
							curShader.transparent = false;
						break;

						case "downscaleShader":
							curShader.transparent = false;
						break;

						case "upscaleShader":
							curShader.transparent = false;
						break;

						case "textShader":
							curShader.transparent = false;
						break;

						case "bgShader":
							curShader.transparent = false;
						break;

						case "layerShader":
							curShader.transparent = false;
						break;

						case "aoShader":
							curShader.transparent = false;
						break;
						case "lightingShader":
							curShader.transparent = false;
						break;
						case "reflShader":
							curShader.transparent = false;
						break;


					}
					
					
					
					curShader.side = THREE.DoubleSide;
					curShader.depthWrite = false;

					curShader.vertexShader = splitText[0] + splitText[1] + splitText[2];
					curShader.fragmentShader = splitText[0] + splitText[3];
					curShader.uniforms = gob.getUniforms(splitText[0],"uniform");
					curShader.attributes = gob.getUniforms(splitText[1],"attribute");

					gob.materials[gob.shaderNames[i]] = new THREE.ShaderMaterial(curShader);


				},
				async:   false,
				dataType:"text"
			});

		}

		for (i in g_fonts) {
			if (g_fonts.hasOwnProperty(i)) {
				gob.getKernMap(g_fonts[i]);
			}
		}


		gob.initScene();


		j$(document).ready(function(){
			j$(document).mousemove(function(e){

				var x = ((e.pageX/window.innerWidth)-0.5)*2.0;
				var y = -((e.pageY/window.innerHeight)-0.5)*2.0;

				var oldRendering = gob.isRendering;

				var maxval = 0.9;

				gob.isRendering = (x < maxval && x > -maxval && y < maxval && y > -maxval);

				if (oldRendering != gob.isRendering) {
					console.log("gob.isRendering: " + gob.isRendering);
				}

				//TODO: update rendering on mouse movement instead


				gob.shaders.lightingShader.uniforms.u_MouseCoords.value.x = x;
				gob.shaders.lightingShader.uniforms.u_MouseCoords.value.y = y;

				//gob.shaders.textShader.uniforms.u_MouseCoords.value.x = e.pageX;
				//gob.shaders.textShader.uniforms.u_MouseCoords.value.y = e.pageY;
				//gob.shaders.bgShader.uniforms.u_MouseCoords.value.x = e.pageX;
				//gob.shaders.bgShader.uniforms.u_MouseCoords.value.y = e.pageY;
			});
			j$(document).mousedown(function(e){
				
				var wRatio = e.pageX/window.innerWidth;
				var hRatio = e.pageY/window.innerHeight;

				if (gob.debugTex) {
					if (gob.curDebugSection == 0.0) {
						if (hRatio < 0.5) {
							if (wRatio < 0.5) {
								gob.curDebugSection = 1.0;
							}
							else {
								gob.curDebugSection = 2.0;
							}
						}
						else {
							if (wRatio < 0.5) {
								gob.curDebugSection = 3.0;
							}
							else {
								gob.curDebugSection = 4.0;
							}
						}
					}
					else {
						gob.curDebugSection = 0.0;
					}

					gob.shaders.debugShader.uniforms.u_Section.value = gob.curDebugSection;

				}
				else {
					testObj.maxWidth = (e.pageX-testObj.x)/zoom;
					testObj.maxHeight = (e.pageY-testObj.y)/zoom;

					//(obj, isBG, firstRun, geoBuffer)
					gob.doTextUpdate();
				}

				


				
			});

			j$(document).mousewheel(function(event, delta, deltaX, deltaY) {

				//zoom += deltaY/100.0;

			});



			j$(document).keypress(function(e) {

				var code = (e.keyCode ? e.keyCode : e.which);

				if ( code == "s".charCodeAt(0) ) {
					//gob.isRendering = !gob.isRendering;

					if (gob.isRendering) {
						console.log("animation resumed");
					}
					else {
						console.log("animation stopped");
					}

				}
			});



		})

		gob.animate();
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
			gob.doError("Browser does not support WebSockets");
			return;
		}
		if ( ! Detector.webgl ) {
			gob.doError("Browser does not support WebGL");
		}

		window.onbeforeunload = function() {
		    gob.connection.onclose = function () {}; // disable onclose handler first
		    gob.connection.close()
		};

		//renderer.deallocateRenderTarget(gob.renderTargets.baseRT);
		gob.renderTargets.baseRT = new THREE.WebGLRenderTarget( window.innerWidth, window.innerHeight, {
			minFilter: THREE.LinearFilter, // NearestFilter // LinearFilter
			magFilter: THREE.LinearFilter, 
			format: THREE.RGBAFormat
		} );
		gob.renderTargets.layerRT = new THREE.WebGLRenderTarget( window.innerWidth, window.innerHeight, {
			minFilter: THREE.LinearFilter, // NearestFilter // LinearFilter
			magFilter: THREE.LinearFilter, 
			format: THREE.RGBAFormat
		} );
		gob.renderTargets.aoRT = new THREE.WebGLRenderTarget( window.innerWidth, window.innerHeight, {
			minFilter: THREE.LinearFilter, // NearestFilter // LinearFilter
			magFilter: THREE.LinearFilter, 
			format: THREE.RGBAFormat
		} );

		gob.renderTargets.lightingRT = new THREE.WebGLRenderTarget( window.innerWidth, window.innerHeight, {
			minFilter: THREE.LinearFilter, // NearestFilter // LinearFilter
			magFilter: THREE.LinearFilter, 
			format: THREE.RGBAFormat
		} );

		gob.renderTargets.downscaleRT = new THREE.WebGLRenderTarget( window.innerWidth/2, window.innerHeight/2, {
			minFilter: THREE.NearestFilter, // NearestFilter // LinearFilter
			magFilter: THREE.NearestFilter, 
			format: THREE.RGBAFormat
		} );

		

		gob.curFLIndex = 0;

		


		


		for (i = 0; i < gob.fontNames.length; i++) {

			var cfName = gob.fontNames[i];

			g_fonts[cfName].heightRT = new THREE.WebGLRenderTarget( g_fonts[cfName].texture.width,g_fonts[cfName].texture.height, {
				minFilter: THREE.LinearFilter, // NearestFilter // LinearFilter
				magFilter: THREE.LinearFilter, 
				format: THREE.RGBAFormat
			} );
			g_fonts[cfName].normRT = new THREE.WebGLRenderTarget( g_fonts[cfName].texture.width,g_fonts[cfName].texture.height, {
				minFilter: THREE.LinearFilter, 
				magFilter: THREE.LinearFilter, 
				format: THREE.RGBAFormat
			} );
			g_fonts[cfName].normHalfRT = new THREE.WebGLRenderTarget( g_fonts[cfName].texture.width/2,g_fonts[cfName].texture.height/2, {
				minFilter: THREE.LinearFilter, 
				magFilter: THREE.LinearFilter, 
				format: THREE.RGBAFormat
			} );
		}


		var loadFont = function() {

			g_fonts[gob.fontNames[ gob.curFLIndex] ].texData = THREE.ImageUtils.loadTexture( './fonts/'+gob.fontNames[gob.curFLIndex]+'.png', new THREE.UVMapping(), function() {

				gob.curFLIndex++;

				if (gob.curFLIndex == gob.fontNames.length) {
					gob.initFinal();
				}
				else {
					loadFont();
				}

			});

		}

		loadFont();

	}
	
	gob.drawTextArea = function(obj, isBG, firstRun, curMesh) {

		curMesh.curInd = 0;

		var curShader;
		var str = obj.str;
		var x = obj.x;
		var y = obj.y;
		var scale = obj.scale;
		var pMaxW = obj.maxWidth;
		var pMaxH = obj.maxHeight;
		//var curFont = obj.font;

		var xOff;
		var yOff;

		var maxW = (pMaxW - obj.padding*2.0)/scale;
		var maxH = (pMaxH - obj.padding*2.0)/scale;
		var i;
		var j;
		var k;
		var lineArr = str.split('\n');
		var wordArr = [];
		var wordObjArr = [];
		var finalLineArr = [];
		
		
		var curChar;
		var charCode;
		var curWordObj;
		var curWordWidth;
		var curWordStr;
		var curLineWidth;
		var nextCharCode;
		var resSourceX = gob.curFont.texture.width;
		var resSourceY = gob.curFont.texture.height;
		var curLineStr;
		var spaceWidth = gob.curFont.chars[0].width;
		var dashWidth = gob.curFont.chars[13].width;
		var wordMod;

		var hAlign = obj.hAlign ? obj.hAlign:0;
		var vAlign = obj.vAlign ? obj.vAlign:0;
		var tempWordStart;

		var curInd0;
		var curInd1;
		var curInd2;
		var curInd3;
		var curIndDiv4;

		var extraInd = 0;

		var dStr;


		for (i = 0; i < lineArr.length; i++) {
			wordArr.push(lineArr[i].split(' '));
		}

		// determine width of each word
		for (i = 0; i < wordArr.length; i++) {
			wordObjArr.push([]);
			
			for (j = 0; j < wordArr[i].length; j++) {

				curWordWidth = 0;
				tempWordStart = 0;
				curWordStr = wordArr[i][j];



				for (k = 0; k < curWordStr.length; k++) {

					charCode = curWordStr.charCodeAt(k);
					curChar = gob.curFont.chars[charCode-32];

					
					if (k < curWordStr.length - 1) {
						nextCharCode = curWordStr.charCodeAt(k+1);
						curWordWidth += curChar.width + gob.curFont.kernMap[charCode*128 + nextCharCode];
					}
					else {
						curWordWidth += curChar.width;
					}


					if (curWordWidth + dashWidth*3 > maxW) {

						dStr = "-";
						if (k+1 == curWordStr.length) {
							dStr = "";
						}

						wordObjArr[i].push({
							wordStr:curWordStr.substring(tempWordStart,k+1)+dStr,
							wordWidth:curWordWidth+dashWidth
						});
						curWordWidth = 0;
						tempWordStart = k+1;


					}

				}

				

				if (curWordWidth > 0) {


					if (tempWordStart == 0) {
						wordObjArr[i].push({
							wordStr:curWordStr,
							wordWidth:curWordWidth
						});
					}
					else {
						wordObjArr[i].push({
							wordStr:curWordStr.substring(tempWordStart,k+1),
							wordWidth:curWordWidth
						});
					}

					
				}
				


				
			}
		}

		// determine width of each line
		for (i = 0; i < wordObjArr.length; i++) {
			
			curLineStr = "";
			curLineWidth = 0;


			for (j = 0; j < wordObjArr[i].length; j++) {

				curWordObj = wordObjArr[i][j];

				if (curLineWidth + curWordObj.wordWidth + spaceWidth > maxW) {
					
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

		// trim off newlines at start/end;

		while (finalLineArr.last().lineWidth == 0) {
			finalLineArr.pop();
		}

		while (finalLineArr[0].lineWidth == 0) {
			finalLineArr.shift();
		}


		if (isBG && obj.drawBG) {
			curShader = "bgShader";
			atArr[0] =  obj.maxWidth;  atArr[1] = obj.maxHeight;  atArr[2] = 0.0;  atArr[3] = 1.0;
			atArr[4] =  obj.maxWidth;  atArr[5] = obj.maxHeight;  atArr[6] = 0.0;  atArr[7] = 1.0;
			atArr[8] =  obj.maxWidth;  atArr[9] = obj.maxHeight;  atArr[10] = 0.0; atArr[11] = 1.0;
			atArr[12] = obj.maxWidth; atArr[13] = obj.maxHeight; atArr[14] = 0.0; atArr[15] = 1.0;
			
			gob.drawString(obj,true, "", x, y, 0, 0, firstRun, curMesh);
		}
		else {
			curShader = "textShader";
			atArr[0] = 0.0;  atArr[1] = 0.0;  atArr[2] = 0.0;  atArr[3] = 0.0;
			atArr[4] = 0.0;  atArr[5] = 0.0;  atArr[6] = 0.0;  atArr[7] = 0.0;
			atArr[8] = 0.0;  atArr[9] = 0.0;  atArr[10] = 0.0; atArr[11] = 0.0;
			atArr[12] = 0.0; atArr[13] = 0.0; atArr[14] = 0.0; atArr[15] = 0.0;

			
			for (i = 0; i < finalLineArr.length; i++) {
				yOff = i*gob.curFont.metrics.ascender + gob.curFont.metrics.descender;
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
				

				if ( (i+1)*gob.curFont.metrics.ascender - gob.curFont.metrics.descender < maxH) {
					gob.drawString(obj,false, finalLineArr[i].lineStr, x, y, xOff, yOff, firstRun, curMesh);
				}
				else {
					extraInd += finalLineArr[i].lineStr.length*4;
				}
			}
		}


		
		

		if (firstRun) {
			curMesh.maxInd = Math.floor( (curMesh.curInd+extraInd) *1.5);

			for (i = curMesh.curInd; i < curMesh.maxInd; i += 4) {
				curMesh.geometry.vertices.push(
					new THREE.Vector3( 0, 0, 0 ),
					new THREE.Vector3( 0, 0, 0 ),
					new THREE.Vector3( 0, 0, 0 ),
					new THREE.Vector3( 0, 0, 0 )
				);

				curMesh.geometry.faceVertexUvs[0].push([
					new THREE.Vector2( 0, 0 ),
					new THREE.Vector2( 0, 0 ),
					new THREE.Vector2( 0, 0 ),
					new THREE.Vector2( 0, 0 )
				]);

				gob.shaders[curShader].attributes.a_Data0.value.push(

					new THREE.Vector4( 0, 0, 0, 0 ),
					new THREE.Vector4( 0, 0, 0, 0 ),
					new THREE.Vector4( 0, 0, 0, 0 ),
					new THREE.Vector4( 0, 0, 0, 0 )

				);

				curMesh.geometry.faces.push( new THREE.Face4(i+0, i+1, i+2, i+3) );
			}

		}
		else {
			for (i = curMesh.curInd; i < curMesh.maxInd; i += 4) {

				curInd0 = i;
				curInd1 = i+1;
				curInd2 = i+2;
				curInd3 = i+3;
				curIndDiv4 = Math.floor(curInd0/4);

				curMesh.geometry.vertices[curInd0].set( 0, 0, 0 );
				curMesh.geometry.vertices[curInd1].set( 0, 0, 0 );
				curMesh.geometry.vertices[curInd2].set( 0, 0, 0 );
				curMesh.geometry.vertices[curInd3].set( 0, 0, 0 );

				curMesh.geometry.faceVertexUvs[0][curIndDiv4][0].set( 0, 0 );
				curMesh.geometry.faceVertexUvs[0][curIndDiv4][1].set( 0, 0 );
				curMesh.geometry.faceVertexUvs[0][curIndDiv4][2].set( 0, 0 );
				curMesh.geometry.faceVertexUvs[0][curIndDiv4][3].set( 0, 0 );

				gob.shaders[curShader].attributes.a_Data0.value[curInd0].set( 0, 0, 0, 0 );
				gob.shaders[curShader].attributes.a_Data0.value[curInd1].set( 0, 0, 0, 0 );
				gob.shaders[curShader].attributes.a_Data0.value[curInd2].set( 0, 0, 0, 0 );
				gob.shaders[curShader].attributes.a_Data0.value[curInd3].set( 0, 0, 0, 0 );

			}
		}


		


		curMesh.geometry.verticesNeedUpdate = true;
		curMesh.geometry.elementsNeedUpdate = true;
		curMesh.geometry.uvsNeedUpdate = true;
		gob.shaders[curShader].attributes[ "a_Data0" ].needsUpdate = true;

		//curMesh.geometry.morphTargetsNeedUpdate = true;
		//curMesh.geometry.normalsNeedUpdate = true;
		//curMesh.geometry.colorsNeedUpdate = true;
		//curMesh.geometry.tangentsNeedUpdate = true;


	}

	gob.drawString = function(obj, isBG, str, xBase, yBase, xOff, yOff, firstRun, curMesh) {
		var curShader;
		var scale = obj.scale;
		var isRect = obj.isRect;
		//var curFont = obj.font;

		var i;
		var curChar;
		var charCode;
		var nextCharCode;

		var curX = xOff;
		var curY = yOff;
		
		var resSourceX = gob.curFont.texture.width;
		var resSourceY = gob.curFont.texture.height;

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

		

		var curInd0;
		var curInd1;
		var curInd2;
		var curInd3;
		var curIndDiv4;


		var curGBLen;

		var strln = str.length;

		if (isBG) {
			curShader = "bgShader";
			strln = 1;
		}
		else {
			curShader = "textShader";
		}


		for (i = 0; i < strln; i++) {
			

			if (isBG) {

				vx1 = xBase;
				vy1 = yBase;
				vx2 = xBase + obj.maxWidth;
				vy2 = yBase + obj.maxHeight;

				tx1 = -1.0;
				ty1 = -1.0;
				tx2 = 1.0;
				ty2 = 1.0;




			}
			else {
				charCode = str.charCodeAt(i);
				curChar = gob.curFont.chars[charCode-32];

				vx = (xBase + (curX + curChar.ox)*scale) + obj.padding;
				vy = (yBase + (curY + gob.curFont.metrics.height - (curChar.oy) )*scale) + obj.padding;
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

				


				if (i < str.length - 1) {
					nextCharCode = str.charCodeAt(i+1);
					curX += curChar.width + gob.curFont.kernMap[charCode*128 + nextCharCode];
				}
			}

			//%%%


			curInd0 = curMesh.curInd;
			curInd1 = curMesh.curInd+1;
			curInd2 = curMesh.curInd+2;
			curInd3 = curMesh.curInd+3;
			curIndDiv4 = Math.floor(curInd0/4);

			if (firstRun) {
				curMesh.geometry.vertices.push(
					new THREE.Vector3( vx1, vy1, 0 ),
					new THREE.Vector3( vx2, vy1, 0 ),
					new THREE.Vector3( vx2, vy2, 0 ),
					new THREE.Vector3( vx1, vy2, 0 )
				);

				curMesh.geometry.faceVertexUvs[0].push([
					new THREE.Vector2( tx1, ty1 ),
					new THREE.Vector2( tx2, ty1 ),
					new THREE.Vector2( tx2, ty2 ),
					new THREE.Vector2( tx1, ty2 )
				]);


				gob.shaders[curShader].attributes.a_Data0.value.push(

					new THREE.Vector4( atArr[0], atArr[1], atArr[2], atArr[3] ),
					new THREE.Vector4( atArr[4], atArr[5], atArr[6], atArr[7] ),
					new THREE.Vector4( atArr[8], atArr[9], atArr[10], atArr[11] ),
					new THREE.Vector4( atArr[12], atArr[13], atArr[14], atArr[15] )

				);

				curGBLen = curMesh.geometry.vertices.length - 4;
				curMesh.geometry.faces.push( new THREE.Face4(curGBLen+0, curGBLen+1, curGBLen+2, curGBLen+3) );
			}
			else {

				if (curInd0 < curMesh.maxInd) {
					curMesh.geometry.vertices[curInd0].set( vx1, vy1, 0 );
					curMesh.geometry.vertices[curInd1].set( vx2, vy1, 0 );
					curMesh.geometry.vertices[curInd2].set( vx2, vy2, 0 );
					curMesh.geometry.vertices[curInd3].set( vx1, vy2, 0 );

					curMesh.geometry.faceVertexUvs[0][curIndDiv4][0].set( tx1, ty1 );
					curMesh.geometry.faceVertexUvs[0][curIndDiv4][1].set( tx2, ty1 );
					curMesh.geometry.faceVertexUvs[0][curIndDiv4][2].set( tx2, ty2 );
					curMesh.geometry.faceVertexUvs[0][curIndDiv4][3].set( tx1, ty2 );

					gob.shaders[curShader].attributes.a_Data0.value[curInd0].set( atArr[0], atArr[1], atArr[2], atArr[3] );
					gob.shaders[curShader].attributes.a_Data0.value[curInd1].set( atArr[4], atArr[5], atArr[6], atArr[7] );
					gob.shaders[curShader].attributes.a_Data0.value[curInd2].set( atArr[8], atArr[9], atArr[10], atArr[11] );
					gob.shaders[curShader].attributes.a_Data0.value[curInd3].set( atArr[12], atArr[13], atArr[14], atArr[15] );

					//curMesh.geometry.faces[ curIndDiv4 ].set(curInd0, curInd1, curInd2, curInd3);
				}
				else {
					gob.doError("Exceeded Buffer Length");
				}

				


			}

			curMesh.curInd += 4;

		}

	}

	gob.onWindowResize = function( event ) {

		var i;
		
		
		for (i = 0; i < gob.shaderNames.length; i++) {
			gob.shaders[gob.shaderNames[i]].uniforms.u_Resolution.value.x = window.innerWidth;
			gob.shaders[gob.shaderNames[i]].uniforms.u_Resolution.value.y = window.innerHeight;
		}

		gob.shaders.downscaleShader.uniforms.u_TexResolution.value.x = Math.floor(window.innerWidth/2);
		gob.shaders.downscaleShader.uniforms.u_TexResolution.value.y = Math.floor(window.innerHeight/2);
		gob.shaders.upscaleShader.uniforms.u_TexResolution.value.x = Math.floor(window.innerWidth/2);
		gob.shaders.upscaleShader.uniforms.u_TexResolution.value.y = Math.floor(window.innerHeight/2);

		renderer.setSize( window.innerWidth, window.innerHeight );


	}

	gob.animate = function() {

		var i;

		requestAnimationFrame( gob.animate );

		if (gob.isRendering) {
			
			for (i = 0; i < gob.shaderNames.length; i++) {
				gob.shaders[gob.shaderNames[i]].uniforms.u_Zoom.value = zoom;
			}

			if (gob.debugTex) {
				gob.debugTexture();
			}
			else {

				if (gob.updateBaseRT) {

					//gl.disable(gl.BLEND);
					//gl.enable(gl.DEPTH_TEST);

					gob.updateBaseRT = false;
					
					renderer.render( scene, camera, gob.renderTargets.baseRT, true );
					
					gob.renderLayers();

					gob.renderToTarget(
						"aoShader",
						gob.renderTargets.aoRT,
						[
							gob.renderTargets.layerRT
						]
					);
					

				}
				else {
					
					gob.renderToTarget(
						"lightingShader",
						undefined,//gob.renderTargets.lightingRT,
						[
							gob.renderTargets.aoRT,
							gob.renderTargets.layerRT
						]
					);

					/*
					gob.renderToTarget(
						"downscaleShader",
						gob.renderTargets.downscaleRT,
						[
							gob.renderTargets.lightingRT
						]
					);

					gob.renderToTarget(
						"upscaleShader",
						undefined,
						[
							gob.renderTargets.downscaleRT,
							gob.renderTargets.layerRT

						]
					);
					*/

					
				}

				

				//gob.renderTargets.baseRT
				//renderer.render( scene, camera );
			}

		}

		stats.update();

		

	}

	gob.renderLayers = function() { //shad, renderTarg, textureArr

		gob.shaders.layerShader.uniforms.u_Texture0.value = gob.renderTargets.baseRT;
		gob.shaders.layerShader.uniforms.u_Time.value = performance.now();
		renderer.render( layerScene, camera, gob.renderTargets.layerRT, true );
	}

	gob.renderToTarget = function(shad, renderTarg, textureArr) {

		var i;
		meshFSQ.material = gob.materials[shad];

		for (i = 0; i < textureArr.length; i++) {
			gob.shaders[shad].uniforms["u_Texture"+i].value = textureArr[i];
		}

		renderer.render( rtScene, camera, renderTarg, true );
	}

	gob.debugTexture = function() {
		meshFSQ.material = gob.materials["debugShader"];
		gob.shaders["debugShader"].uniforms.u_Texture0.value = gob.debugTex;
		renderer.render( rtScene, camera );
	}


	gob.setCurFont = function(fName) {
		gob.curFont = fName;
		gob.shaders.textShader.uniforms.u_Texture0.value = gob.curFont.heightRT;
		gob.shaders.textShader.uniforms.u_Texture1.value = gob.curFont.normHalfRT;


		gob.shaders.heightShader.uniforms.u_TexResolution.value.x = gob.curFont.texture.width;
		gob.shaders.heightShader.uniforms.u_TexResolution.value.y = gob.curFont.texture.height;
		gob.shaders.normShader.uniforms.u_TexResolution.value.x = gob.curFont.texture.width;
		gob.shaders.normShader.uniforms.u_TexResolution.value.y = gob.curFont.texture.height;

		gob.shaders.downscaleShader.uniforms.u_TexResolution.value.x = gob.curFont.texture.width;
		gob.shaders.downscaleShader.uniforms.u_TexResolution.value.y = gob.curFont.texture.height;
		gob.shaders.upscaleShader.uniforms.u_TexResolution.value.x = gob.curFont.texture.width;
		gob.shaders.upscaleShader.uniforms.u_TexResolution.value.y = gob.curFont.texture.height;

		gob.renderToTarget(
			"heightShader",
			gob.curFont.heightRT,
			[
				gob.curFont.texData
			]
		);

		gob.renderToTarget(
			"normShader",
			gob.curFont.normRT,
			[
				gob.curFont.texData,
				gob.curFont.heightRT
			]
		);

		gob.renderToTarget(
			"downscaleShader",
			gob.curFont.normHalfRT,
			[
				gob.curFont.normRT
			]
		);

	}

	gob.initScene = function() {

		var i;

		container = document.getElementById( 'container' );
		camera = new THREE.Camera();
		camera.position.z = 1;
		scene = new THREE.Scene();
		
		var gbText = new THREE.Geometry();
		gbText.dynamic = true;
		meshText = new THREE.Mesh( gbText, gob.materials.textShader);
		meshText.curInd = 0;
		meshText.maxInd = 0;

		var gbBG = new THREE.Geometry();
		gbBG.dynamic = true;
		meshBG = new THREE.Mesh( gbBG, gob.materials.bgShader);
		meshBG.curInd = 0;
		meshBG.maxInd = 0;
		


		rtScene = new THREE.Scene();
		layerScene = new THREE.Scene();


		renderer = new THREE.WebGLRenderer();
		renderer.sortObjects = false;
		renderer.autoClear = false;

		var geoBufferFSQ = new THREE.Geometry();
		meshFSQ = new THREE.Mesh( geoBufferFSQ);
		geoBufferFSQ.vertices.push(
			new THREE.Vector3( -1, -1, 0 ),
			new THREE.Vector3(  1, -1, 0 ),
			new THREE.Vector3(  1,  1, 0 ),
			new THREE.Vector3( -1,  1, 0 )
		);
		geoBufferFSQ.faceVertexUvs[0].push([
			new THREE.Vector2( 0, 0 ),
			new THREE.Vector2( 1, 0 ),
			new THREE.Vector2( 1, 1 ),
			new THREE.Vector2( 0, 1 )
		]);
		geoBufferFSQ.faces.push( new THREE.Face4(0, 1, 2, 3) );
		rtScene.add(meshFSQ);

		var geoBufferLayer = new THREE.Geometry();
		
		var curh;
		var curd;
		var layerScale;
		for (i = 0; i < gob.maxLayers; i++) {
			curh = 2.0*i/window.innerHeight;
			curd = i/255.0;
			//layerScale = 1.0 + (i/gob.maxLayers)*( gob.maxLayers/Math.min(window.innerHeight,window.innerWidth) );
			geoBufferLayer.vertices.push(
				new THREE.Vector3( -1.0, -1.0+curh, curd ),
				new THREE.Vector3(  1.0, -1.0+curh, curd ),
				new THREE.Vector3(  1.0,  1.0+curh, curd ),
				new THREE.Vector3( -1.0,  1.0+curh, curd )
			);
			geoBufferLayer.faceVertexUvs[0].push([
				new THREE.Vector2( 0, 0 ),
				new THREE.Vector2( 1, 0 ),
				new THREE.Vector2( 1, 1 ),
				new THREE.Vector2( 0, 1 )
			]);
			geoBufferLayer.faces.push( new THREE.Face4(i*4+0, i*4+1, i*4+2, i*4+3) );
		}
		meshLayer = new THREE.Mesh( geoBufferLayer);
		meshLayer.material = gob.materials["layerShader"];
		layerScene.add(meshLayer);



		
		gob.setCurFont(g_fonts["arial_black_regular_96"]);
		


		
		testObj = {

			str: 			"Lorem ipsum dolor sit amet, pro nostrum ullamcorper at,\n\n est meis mediocritatem eu. No ludus zril quando eum, et altera aliquam menandri per, ad his ridens discere efficiendi. An quo detracto vituperata, pro regione detracto abhorreant no, tantas sententiae te mei. Pri suavitate conclusionemque te, enim laoreet per te. Utinam aliquam detracto te sea. Lorem ipsum dolor sit amet, pro graeci nostrum ullamcorper at,\n\n est meis mediocritatem eu. No ludus zril quando eum, et altera aliquam menandri per, ad his ridens discere efficiendi. An quo detracto vituperata, pro regione detracto abhorreant no, tantas sententiae te mei. Pri suavitate conclusionemque te, enim laoreet per te. Utinam aliquam detracto te sea. Lorem ipsum dolor sit amet, pro graeci nostrum ullamcorper at,\n\n est meis mediocritatem eu. No ludus zril quando eum, et altera aliquam menandri per, ad his ridens discere efficiendi. An quo detracto vituperata, pro regione detracto abhorreant no, tantas sententiae te mei. Pri suavitate conclusionemque te, enim laoreet per te. Utinam aliquam detracto te sea.",
			font: 			gob.curFont,
			x: 				50,
			y: 				50,
			scale: 			1.0,
			maxWidth: 		700,
			maxHeight: 		550,
			hAlign: 		0,
			vAlign: 		0,
			drawBG: 		true,
			
			cornerRad: 		8,
			padding: 		16,
			border: 		0,
			margin: 		0,
			zdepth: 		0
		};

		//(obj, isBG, firstRun, geoBuffer)
		gob.drawTextArea(testObj, true, true, meshBG);
		gob.drawTextArea(testObj, false, true, meshText);
		
		
		scene.add( meshText );
		scene.add( meshBG );
		
		
		container.appendChild( renderer.domElement );
		stats = new Stats();
		stats.domElement.style.position = 'absolute';
		stats.domElement.style.bottom = '0px';
		//container.appendChild( stats.domElement );
		gob.onWindowResize();
		window.addEventListener( 'resize', gob.onWindowResize, false );


	}

	gob.setFont = function(val) {
		//testObj.scale = val;
		gob.setCurFont(val);
		gob.doTextUpdate();
		
	}

	gob.getKernMap = function(fontObj) {
		var i;
		var curKern;
		var totSize = 128*128;

		fontObj.kernMap = new Array(totSize);

		for (i = 0; i < totSize; i++) {
			fontObj.kernMap[i] = 0;
		}

		for (i = 0; i < fontObj.kernings.length; i++) {
			curKern = fontObj.kernings[i];

			fontObj.kernMap[ curKern.from.charCodeAt(0)*128 + curKern.to.charCodeAt(0) ] = curKern.offset;

		}

	}

	gob.getUniforms = function(str, varType) {
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
								resObj[curName] = { type: "t" };//, value: curMap };
							break;
							default:
								gob.doError("Invalid type: " + curType);
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
								gob.doError("Invalid type: " + curType);
							break;
						}
					}

					

				}
				else {
					gob.doError('Invalid uniform: ' + newStr);
				}
			}
		}

		return resObj;
	}


	

	gob.init();



});