
var gob;

if(!Array.prototype.last) {
    Array.prototype.last = function() {
        return this[this.length - 1];
    }
}

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
	
	var rtScene;
	
	var mesh;
	var meshFSQ;
	var geoBuffer;

	gob = {

		shaderNames:["textShader","heightShader","normShader"],
		fontNames:["arial_black_regular_48"],
		fontLoaded:{},
		shaders:{},
		materials:{},


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

	gob.initFinal = function() {
		var curShader;
		var i;

		


		
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
						
						case "heightShader":
							curShader.transparent = false;
						break;

						case "normShader":
							curShader.transparent = false;
						break;

						case "textShader":
							curShader.transparent = true;
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
				
			});
			j$(document).mousedown(function(e){
				
				testObj.maxWidth = e.pageX-testObj.x;
				testObj.maxHeight = e.pageY-testObj.y;

				gob.drawTextArea(testObj, false);
			});

			j$(document).mousewheel(function(event, delta, deltaX, deltaY) {

				zoom += deltaY/100.0;

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


		for (i = 0; i < gob.fontNames.length; i++) {

			var cfName = gob.fontNames[i];

			g_fonts[cfName].heightRT = new THREE.WebGLRenderTarget( g_fonts[cfName].texture.width,g_fonts[cfName].texture.height, {
				minFilter: THREE.LinearFilter, 
				magFilter: THREE.LinearFilter, 
				format: THREE.RGBAFormat
			} );
			g_fonts[cfName].normRT = new THREE.WebGLRenderTarget( g_fonts[cfName].texture.width,g_fonts[cfName].texture.height, {
				minFilter: THREE.LinearFilter, 
				magFilter: THREE.LinearFilter, 
				format: THREE.RGBAFormat
			} );

			g_fonts[cfName].texData = THREE.ImageUtils.loadTexture( './fonts/'+gob.fontNames[i]+'.png', new THREE.UVMapping(), function() {
				gob.fontLoaded[cfName] = true;
				var allLoaded = true;
				var k;
				for (k = 0; k < gob.fontNames.length; k++) {
					if (gob.fontLoaded[ gob.fontNames[k] ]) {

					}
					else {
						allLoaded = false;
					}
				}

				if (allLoaded) {
					gob.initFinal();
				}

			} );
		}

	}
	
	gob.drawTextArea = function(obj, firstRun) {

		obj.curInd = 0;

		var str = obj.str;
		var x = obj.x;
		var y = obj.y;
		var scale = obj.scale;
		var pMaxW = obj.maxWidth;
		var pMaxH = obj.maxHeight;
		//var curFont = obj.font;

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

						wordObjArr[i].push({
							wordStr:curWordStr.substring(tempWordStart,k+1)+"-",
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

		// trim off newlines at end;

		while (finalLineArr.last().lineWidth == 0) {
			finalLineArr.pop();
		}


		if (obj.drawBG) {
			gob.drawString(obj,true, "", x, y, 0, 0, firstRun);
		}

		
		for (i = 0; i < finalLineArr.length; i++) {
			yOff = (i*gob.curFont.metrics.ascender) + gob.curFont.metrics.descender;
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
			

			if ( (i+1)*gob.curFont.metrics.ascender < maxH) {
				gob.drawString(obj,false, finalLineArr[i].lineStr, x, y, xOff, yOff, firstRun);
			}
			else {
				extraInd += finalLineArr[i].lineStr.length*4;
			}


		}

		if (firstRun) {
			obj.maxInd = Math.floor( (obj.curInd+extraInd) *1.5);

			for (i = obj.curInd; i < obj.maxInd; i += 4) {
				geoBuffer.vertices.push(
					new THREE.Vector3( 0, 0, 0 ),
					new THREE.Vector3( 0, 0, 0 ),
					new THREE.Vector3( 0, 0, 0 ),
					new THREE.Vector3( 0, 0, 0 )
				);

				geoBuffer.faceVertexUvs[0].push([
					new THREE.Vector2( 0, 0 ),
					new THREE.Vector2( 0, 0 ),
					new THREE.Vector2( 0, 0 ),
					new THREE.Vector2( 0, 0 )
				]);

				gob.shaders.textShader.attributes.a_Data0.value.push(

					new THREE.Vector4( 0, 0, 0, 0 ),
					new THREE.Vector4( 0, 0, 0, 0 ),
					new THREE.Vector4( 0, 0, 0, 0 ),
					new THREE.Vector4( 0, 0, 0, 0 )

				);

				geoBuffer.faces.push( new THREE.Face4(i+0, i+1, i+2, i+3) );
			}

		}
		else {
			for (i = obj.curInd; i < obj.maxInd; i += 4) {

				curInd0 = i;
				curInd1 = i+1;
				curInd2 = i+2;
				curInd3 = i+3;
				curIndDiv4 = Math.floor(curInd0/4);

				geoBuffer.vertices[curInd0].set( 0, 0, 0 );
				geoBuffer.vertices[curInd1].set( 0, 0, 0 );
				geoBuffer.vertices[curInd2].set( 0, 0, 0 );
				geoBuffer.vertices[curInd3].set( 0, 0, 0 );

				geoBuffer.faceVertexUvs[0][curIndDiv4][0].set( 0, 0 );
				geoBuffer.faceVertexUvs[0][curIndDiv4][1].set( 0, 0 );
				geoBuffer.faceVertexUvs[0][curIndDiv4][2].set( 0, 0 );
				geoBuffer.faceVertexUvs[0][curIndDiv4][3].set( 0, 0 );

				gob.shaders.textShader.attributes.a_Data0.value[curInd0].set( 0, 0, 0, 0 );
				gob.shaders.textShader.attributes.a_Data0.value[curInd1].set( 0, 0, 0, 0 );
				gob.shaders.textShader.attributes.a_Data0.value[curInd2].set( 0, 0, 0, 0 );
				gob.shaders.textShader.attributes.a_Data0.value[curInd3].set( 0, 0, 0, 0 );

			}
		}


		


		mesh.geometry.verticesNeedUpdate = true;
		mesh.geometry.elementsNeedUpdate = true;
		mesh.geometry.uvsNeedUpdate = true;
		gob.shaders.textShader.attributes[ "a_Data0" ].needsUpdate = true;

		//mesh.geometry.morphTargetsNeedUpdate = true;
		//mesh.geometry.normalsNeedUpdate = true;
		//mesh.geometry.colorsNeedUpdate = true;
		//mesh.geometry.tangentsNeedUpdate = true;


	}

	gob.drawString = function(obj, isBG, str, xBase, yBase, xOff, yOff, firstRun) {

		var scale = obj.scale;
		var isRect = obj.isRect;
		//var curFont = obj.font;


		var curUniforms = gob.shaders.textShader.uniforms;

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

		var at0;
		var at1;
		var at2;
		var at3;

		var curInd0;
		var curInd1;
		var curInd2;
		var curInd3;
		var curIndDiv4;


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
				curChar = gob.curFont.chars[charCode-32];

				vx = (xBase + (curX + curChar.ox)*scale);
				vy = (yBase + (curY + gob.curFont.metrics.height - (curChar.oy) )*scale);
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
					curX += curChar.width + gob.curFont.kernMap[charCode*128 + nextCharCode];
				}
			}


			curInd0 = obj.curInd;
			curInd1 = obj.curInd+1;
			curInd2 = obj.curInd+2;
			curInd3 = obj.curInd+3;
			curIndDiv4 = Math.floor(curInd0/4);

			if (firstRun) {
				geoBuffer.vertices.push(
					new THREE.Vector3( vx1, vy1, 0 ),
					new THREE.Vector3( vx2, vy1, 0 ),
					new THREE.Vector3( vx2, vy2, 0 ),
					new THREE.Vector3( vx1, vy2, 0 )
				);

				geoBuffer.faceVertexUvs[0].push([
					new THREE.Vector2( tx1, ty1 ),
					new THREE.Vector2( tx2, ty1 ),
					new THREE.Vector2( tx2, ty2 ),
					new THREE.Vector2( tx1, ty2 )
				]);

				gob.shaders.textShader.attributes.a_Data0.value.push(

					new THREE.Vector4( at0, at1, at2, at3 ),
					new THREE.Vector4( at0, at1, at2, at3 ),
					new THREE.Vector4( at0, at1, at2, at3 ),
					new THREE.Vector4( at0, at1, at2, at3 )

				);

				curGBLen = geoBuffer.vertices.length - 4;
				geoBuffer.faces.push( new THREE.Face4(curGBLen+0, curGBLen+1, curGBLen+2, curGBLen+3) );
			}
			else {

				if (curInd0 < obj.maxInd) {
					geoBuffer.vertices[curInd0].set( vx1, vy1, 0 );
					geoBuffer.vertices[curInd1].set( vx2, vy1, 0 );
					geoBuffer.vertices[curInd2].set( vx2, vy2, 0 );
					geoBuffer.vertices[curInd3].set( vx1, vy2, 0 );

					geoBuffer.faceVertexUvs[0][curIndDiv4][0].set( tx1, ty1 );
					geoBuffer.faceVertexUvs[0][curIndDiv4][1].set( tx2, ty1 );
					geoBuffer.faceVertexUvs[0][curIndDiv4][2].set( tx2, ty2 );
					geoBuffer.faceVertexUvs[0][curIndDiv4][3].set( tx1, ty2 );

					gob.shaders.textShader.attributes.a_Data0.value[curInd0].set( at0, at1, at2, at3 );
					gob.shaders.textShader.attributes.a_Data0.value[curInd1].set( at0, at1, at2, at3 );
					gob.shaders.textShader.attributes.a_Data0.value[curInd2].set( at0, at1, at2, at3 );
					gob.shaders.textShader.attributes.a_Data0.value[curInd3].set( at0, at1, at2, at3 );

					//geoBuffer.faces[ curIndDiv4 ].set(curInd0, curInd1, curInd2, curInd3);
				}
				else {
					gob.doError("Exceeded Buffer Length");
				}

				


			}

			obj.curInd += 4;

		}

	}

	gob.onWindowResize = function( event ) {

		var i;
		
		/*
		for (i = 0; i < gob.shaderNames.length; i++) {
			gob.shaders[gob.shaderNames[i]].uniforms.u_Resolution.value.x = window.innerWidth;
			gob.shaders[gob.shaderNames[i]].uniforms.u_Resolution.value.y = window.innerHeight;
		}*/

		gob.shaders.textShader.uniforms.u_Resolution.value.x = window.innerWidth;
		gob.shaders.textShader.uniforms.u_Resolution.value.y = window.innerHeight;

		renderer.setSize( window.innerWidth, window.innerHeight );


	}

	gob.animate = function() {

		requestAnimationFrame( gob.animate );
		

		var i;

		for (i = 0; i < gob.shaderNames.length; i++) {
			gob.shaders[gob.shaderNames[i]].uniforms.u_Zoom.value = zoom;
		}

		
		
		renderer.render( scene, camera );
		
		



		stats.update();

	}

	gob.renderToTarget = function(shad, renderTarg, textureArr) {

		var i;

		meshFSQ.material = gob.materials[shad];

		for (i = 0; i < textureArr.length; i++) {
			gob.shaders[shad].uniforms["u_Texture"+i].value = textureArr[i];
		}

		renderer.render( rtScene, camera, renderTarg );
	}

	gob.setCurFont = function(fName) {
		gob.curFont = fName;
		gob.shaders.textShader.uniforms.u_Texture0.value = gob.curFont.heightRT;
		gob.shaders.textShader.uniforms.u_Texture1.value = gob.curFont.normRT;
	}

	gob.initScene = function() {


		container = document.getElementById( 'container' );
		camera = new THREE.Camera();
		camera.position.z = 1;
		scene = new THREE.Scene();
		geoBuffer = new THREE.Geometry();
		geoBuffer.dynamic = true;
		mesh = new THREE.Mesh( geoBuffer, gob.materials.textShader);

		rtScene = new THREE.Scene();

		renderer = new THREE.WebGLRenderer();
		renderer.autoClear = false;
		
		gob.setCurFont(g_fonts["arial_black_regular_48"])
		
		
		var geoBufferFSQ = new THREE.Geometry();
		meshFSQ = new THREE.Mesh( geoBufferFSQ);

		//
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
		//

		rtScene.add(meshFSQ);

		
		gob.shaders.heightShader.uniforms.u_Resolution.value.x = gob.curFont.texture.width;
		gob.shaders.heightShader.uniforms.u_Resolution.value.y = gob.curFont.texture.height;
		gob.shaders.normShader.uniforms.u_Resolution.value.x = gob.curFont.texture.width;
		gob.shaders.normShader.uniforms.u_Resolution.value.y = gob.curFont.texture.height;

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
		
		
		

		
		testObj = {
			curInd:  		0,
			maxInd: 		0,

			str: 			"Lorem ipsum dolor sit amet, pro test1test2test3test4test5test6test7test8test9test10test11test12test13test14test1test2test3test4test5test6test7test8test9test10test11test12test13test14 nostrum ullamcorper at,\n\n est meis mediocritatem eu. No ludus zril quando eum, et altera aliquam menandri per, ad his ridens discere efficiendi. An quo detracto vituperata, pro regione detracto abhorreant no, tantas sententiae te mei. Pri suavitate conclusionemque te, enim laoreet per te. Utinam aliquam detracto te sea. Lorem ipsum dolor sit amet, pro graeci nostrum ullamcorper at,\n\n est meis mediocritatem eu. No ludus zril quando eum, et altera aliquam menandri per, ad his ridens discere efficiendi. An quo detracto vituperata, pro regione detracto abhorreant no, tantas sententiae te mei. Pri suavitate conclusionemque te, enim laoreet per te. Utinam aliquam detracto te sea. Lorem ipsum dolor sit amet, pro graeci nostrum ullamcorper at,\n\n est meis mediocritatem eu. No ludus zril quando eum, et altera aliquam menandri per, ad his ridens discere efficiendi. An quo detracto vituperata, pro regione detracto abhorreant no, tantas sententiae te mei. Pri suavitate conclusionemque te, enim laoreet per te. Utinam aliquam detracto te sea.",
			font: 			gob.curFont,
			x: 				1,
			y: 				1,
			scale: 			0.25,
			maxWidth: 		400,
			maxHeight: 		400,
			hAlign: 		0,
			vAlign: 		0,
			drawBG: 		true,
			border: 		0,
			padding: 		0,
			margin: 		0,
			zdepth: 		0
		};


		gob.drawTextArea(testObj, true);
		
		

		scene.add( mesh );
		
		container.appendChild( renderer.domElement );
		stats = new Stats();
		stats.domElement.style.position = 'absolute';
		stats.domElement.style.bottom = '0px';
		container.appendChild( stats.domElement );
		gob.onWindowResize();
		window.addEventListener( 'resize', gob.onWindowResize, false );


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