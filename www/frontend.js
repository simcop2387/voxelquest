
var gob;

var g_curGroup = 0;
var a_DataArr = ["a_Data0","a_Data1"];

var enums = {
	fillDir: {
		horizontal:0,
		vertical:1
	},
	align: {
		left:0,
		center:1,
		right:2,

		top:3,
		middle:4,
		bottom:5
	}
}

function logInPlace(obj) {
	//console.log(obj);
	return obj;
}

function combineObjs(oArr) {
	var resObj = {};

	var i;
	var j;
	var curObj;

	if (oArr.length) {

	}
	else {
		return JSON.parse(JSON.stringify(oArr));
	}

	for (i = 0; i < oArr.length; i++) {

		curObj = oArr[i];

		for (j in curObj) {
			if (curObj.hasOwnProperty(j)) {
				resObj[j] = curObj[j];
			}
		}
	}

	return JSON.parse(JSON.stringify(resObj));
	//resObj
}

function createNode(propArr,childArr) {
	return {
		props:combineObjs(propArr),
		childArr:childArr
	}
}

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
	//var testObj;

	var zoom = 1;
	var atArr;
	var atArrs = [
		[0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0],
		[0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0]
	];
	
	var rtScene;
	var layerScene;
	
	var meshText;
	var meshBG;
	var meshFSQ;
	

	gob = {
		lockOn: false,
		activeComp: 0,
		autoUpdate: false,
		curDebugSection:0.0,
		traceVals:[],
		styleSheets:{},
		mainRoot:null,
		bufferWidth: 1024,
		bufferHeight: 768,
		isRendering: true,
		maxLayers: 20,
		updateBaseRT:true,
		renderTargets:{},
		shaderNames:["reflShader","lightingShader","palShader","aoShader","aoHighShader","extrudeShader","layerShader","bgShader","textShader","bgIdShader","textIdShader","heightShader","normShader","downscaleShader","upscaleShader","debugShader"],
		fontNames:["arial_black_regular_48","arial_black_regular_96"],
		fontLoaded:{},
		shaders:{},
		materials:{},
		debugTex:null,
		displayList:[],
		traceOn:false,
		traceLevel: 0,
		lastTraceLevel: 0,
		hasConnection:false,
		tracesAtLevel: 0,
		traceArr: [],
		lastMessage: null,
		maxFrameSize: 16777216,
		doError: function(str) {
			console.log(str);
		},
		pushTrace: function() {
			gob.traceLevel++;
			gob.tracesAtLevel = 0;
		},
		popTrace: function() {
			

			if (gob.traceLevel < gob.lastTraceLevel) {
				gob.doTrace("END");
			}

			gob.traceLevel--;
			if (gob.traceLevel < 0) {
				gob.traceLevel = 0;
			}			

			gob.tracesAtLevel = 0;



			
		},
		doTrace:function(traceStr, otherPrefix) {
			
			var resStr;

			if (gob.traceOn) {
				gob.tracesAtLevel++;

				

				gob.lastTraceLevel = gob.traceLevel;

				var op = otherPrefix;

				if (op) {} else {
					op = "";
				}

				var prefix = gob.getTracePrefix() + op;
				var prefixn = "\n"+prefix;

				if (typeof traceStr == "object") {

					var objStr = JSON.stringify(traceStr,null,"\t");
					var newStr = prefix + objStr.replace(/[\r\n]/g, prefixn);

					resStr = ( newStr );
				}
				else {
					resStr = (prefix + traceStr);
				}
			}
			else {
				resStr = (traceStr);
			}

			console.log(resStr);
			
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
		wf:function(varName, func, avoidArgs) {
			if (gob.traceOn) {
				gob[varName] = function() {

					var argArr = Array.prototype.slice.call(arguments, 0);
					var i;
					gob.pushTrace();

					var aStr = "";

					if (avoidArgs || (gob.traceVals.length > 0) ) {
						for (i = 0; i < gob.traceVals.length; i++) {
							aStr += argArr[0].props[gob.traceVals[i]] + " ";
						}
					}
					else {
						aStr = argArr.join(",");
					}

					gob.doTrace(varName + "("+aStr+")");
					
					var myr = func.apply(func,arguments);
					gob.popTrace();
					return myr;
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


	gob.getStringWidth = function(curParentProps) {
		var i;
		var curChar;
		var charCode;
		var nextCharCode;
		var curX = 0;
		var str = curParentProps.str;

		var totPadding = curParentProps.padding + curParentProps.border;


		for (i = 0; i < str.length; i++) {
			charCode = str.charCodeAt(i);
			curChar = gob.curFont.chars[charCode-32];
			
			if (i < str.length - 1) {
				nextCharCode = str.charCodeAt(i+1);
				curX += curChar.width + gob.curFont.kernMap[charCode*128 + nextCharCode];
			}
			else {
				curX += curChar.width;
			}
		}

		return curX*curParentProps.scale + totPadding*2;
	}







	gob.wf("getMinWidths", function(rootObj) {
		var i;
		var curParentProps = rootObj.props;
		var curChildProps;
		var minWidth = 0;
		var totPadding = curParentProps.padding + curParentProps.border;

		if (rootObj.childArr.length == 0) {
			if (curParentProps.maxLines == 1 && curParentProps.fitText) {
				minWidth = gob.getStringWidth(curParentProps);
			}
		}
		else {

			for (i = 0; i < rootObj.childArr.length; i++) {
				minWidth = Math.max(gob.getMinWidths(rootObj.childArr[i]), minWidth); 
			}

			
		}

		curParentProps.minWidth = minWidth+(totPadding)*(2);// + Math.max(0,rootObj.childArr.length-1) );

		return curParentProps.minWidth;
		
	});



	gob.wf("setResultWidths", function(rootObj) {
		var i;
		var j;
		var curX;

		var curParentProps = rootObj.props;
		var curChildProps;
		var availWidth;

		var totRatio = 0;
		var totFilled = 0;
		var maxWidth;
		var totWidth;
		var wordCount;
		var curLine;

		var startItem;
		var endItem;
		var curLineWidth;

		var totPadding = curParentProps.padding + curParentProps.border;

		rootObj.lineWidthArr = [];
		rootObj.lineArr = [];



		if (rootObj.childArr.length == 0) {
			
		}
		else {


			if (curParentProps.fillDir == enums.fillDir.horizontal) {
				
				
				totWidth = totPadding*2;
				wordCount = 0;
				curLine = 0;


				// determine lines and line widths
				for (i = 0; i < rootObj.childArr.length; i++) {
					
					wordCount++;
					curChildProps = rootObj.childArr[i].props;


					totWidth += curChildProps.minWidth + totPadding;

					while (rootObj.lineArr.length <= curLine) {
						rootObj.lineArr.push([]);
					}

					if (totWidth-totPadding > curParentProps.resultWidth) {

						if (wordCount == 1) {

							rootObj.lineArr[curLine].push(i);
							rootObj.lineWidthArr.push(curParentProps.resultWidth - totPadding*2);
						}
						else {
							i--;
							rootObj.lineWidthArr.push(totWidth - (curChildProps.minWidth + totPadding*2) );
						}
						wordCount = 0;
						totWidth = totPadding*2;
						curLine++;

						
					}
					else {

						rootObj.lineArr[curLine].push(i);
					}
				}


				if (totWidth != totPadding*2) {

					rootObj.lineWidthArr.push(totWidth - totPadding );
				}

				for (j = 0; j < rootObj.lineArr.length; j++) {
					startItem = rootObj.lineArr[j][0];
					endItem = rootObj.lineArr[j].last();
					curLineWidth = rootObj.lineWidthArr[j];
					availWidth = curParentProps.resultWidth - curLineWidth;


					totRatio = 0;
					for (i = startItem; i <= endItem; i++) {
						curChildProps = rootObj.childArr[i].props;
						totRatio += curChildProps.fillRatio;
					}

					totFilled = 0;
					curX = curParentProps.x + totPadding;
					
					
					for (i = startItem; i <= endItem; i++) {
						curChildProps = rootObj.childArr[i].props;
						
						curChildProps.resultWidth = (curChildProps.minWidth + Math.floor(curChildProps.fillRatio*availWidth/totRatio));
						curChildProps.x = curX;
						
						totFilled += curChildProps.resultWidth;

						curX += totPadding + curChildProps.resultWidth;
					}

					//add on left over space from rounding down
					//curChildProps.resultWidth += availWidth - totFilled;


				}

			}
			else {

				for (i = 0; i < rootObj.childArr.length; i++) {



					curChildProps = rootObj.childArr[i].props;
					maxWidth = curParentProps.resultWidth - totPadding*2;

					if (curChildProps.fillRatio > 0) {
						curChildProps.resultWidth = (maxWidth);
					}
					else {
						curChildProps.resultWidth = (Math.min(curChildProps.minWidth,maxWidth));
					}

					rootObj.lineArr.push([i]);
					rootObj.lineWidthArr.push(curChildProps.resultWidth);

				}

				


				for (i = 0; i < rootObj.childArr.length; i++) {
					curChildProps = rootObj.childArr[i].props;
					curChildProps.x = curParentProps.x + totPadding;
				}


			}


			for (i = 0; i < rootObj.childArr.length; i++) {
				gob.setResultWidths(rootObj.childArr[i]);
			}
		}
		
	});

	gob.wf("getMinHeights",function(rootObj) {
		var i;
		var j;
		var curY;
		var curParentProps = rootObj.props;
		var curChildProps;
		var minHeight = 0;
		var totalMinHeight = 0;
		var curMod;

		var totPadding = curParentProps.padding + curParentProps.border;

		curY = curParentProps.y + totPadding;

		if (rootObj.childArr.length == 0) {
			if (curParentProps.maxLines == 1 ) {
				minHeight = gob.curFont.metrics.height*curParentProps.scale;
			}
			else {
				minHeight = gob.drawTextArea(curParentProps, false, false, null, true);
			}
		}
		else {
			
			if (curParentProps.fillDir == enums.fillDir.horizontal) {


				for (j = 0; j < rootObj.lineArr.length; j++) {
					startItem = rootObj.lineArr[j][0];
					endItem = rootObj.lineArr[j].last();

					minHeight = 0;
					for (i = startItem; i <= endItem; i++) {
						curChildProps = rootObj.childArr[i].props;
						curChildProps.y = curY;

						minHeight = Math.max(gob.getMinHeights(rootObj.childArr[i]), minHeight); 

						
					}



					totalMinHeight += minHeight;

					if (j != rootObj.lineArr.length-1) {
						totalMinHeight += totPadding;
					}

					curY += minHeight + totPadding;

				}

				minHeight = totalMinHeight;
			}
			else {

				for (i = 0; i < rootObj.childArr.length; i++) {
					curChildProps = rootObj.childArr[i].props;
					curChildProps.y = curY;

					curMod = gob.getMinHeights(rootObj.childArr[i]);
					


					if (i != rootObj.childArr.length-1) {
						curMod += totPadding;
					}

					minHeight += curMod;

					curY += curMod;


				}
				
			}


			
		}

		curParentProps.minHeight = minHeight+(totPadding)*(2);// + Math.max(0,rootObj.childArr.length-1) );

		curParentProps.resultHeight = curParentProps.minHeight;

		return curParentProps.minHeight;
		
	});


	gob.wf("addMeshes",function(rootObj,firstRun) {
		var i;

		gob.drawTextArea(rootObj.props, true, firstRun, meshBG);
		gob.drawTextArea(rootObj.props, false, firstRun, meshText);

		for (i = 0; i < rootObj.childArr.length; i++) {
			gob.addMeshes(rootObj.childArr[i],firstRun);
		}

	});


	gob.testHit = function(rootObj, mouseX, mouseY) {
		var i;

		var cp = rootObj.props;
		var res;

		if (rootObj.props.isGroup) {

			if (
				mouseX >= cp.x &&
				mouseY >= cp.y &&
				mouseX <= (cp.x + cp.resultWidth) &&
				mouseY <= (cp.y + cp.resultHeight)
			) {
				return cp;
			}
			else {
				return 0;
			}

		}


		for (i = 0; i < rootObj.childArr.length; i++) {
			res = gob.testHit(rootObj.childArr[i], mouseX, mouseY);

			if (res !== 0) {
				return res;
			}
		}

		return 0;
	}

	gob.numberGroups = function(rootObj, newHeight) {
		var i;

		if (rootObj.props.isGroup) {
			rootObj.props.groupId = g_curGroup;
			g_curGroup++;
		}

		rootObj.props.baseDepth = newHeight;

		for (i = 0; i < rootObj.childArr.length; i++) {
			gob.numberGroups(rootObj.childArr[i], newHeight);
		}
	}

	gob.setGroups = function(rootObj, curGroup) {
		var i;
		var newGroup;


		if (rootObj.props.groupId == 0) {
			newGroup = curGroup;
		}
		else {
			newGroup = rootObj.props.groupId;
		}

		rootObj.props.groupId = newGroup;

		for (i = 0; i < rootObj.childArr.length; i++) {
			gob.setGroups(rootObj.childArr[i], newGroup);
		}
	}

	gob.updateActiveComp = function(xval) {
		if (gob.activeComp === 0) {

		}
		else {
			gob.activeComp.curValue = Math.min(Math.max(xval-gob.activeComp.x,0),gob.activeComp.resultWidth)/gob.activeComp.resultWidth;
			//console.log(gob.activeComp.curValue);

			if (gob.lockOn) {

			}
			else {
				gob.lockOn = true;

				gob.layoutGUI(gob.mainRoot,false);
				gob.updateBaseRT = true;

				gob.lockOn = false;
			}

			

		}
	}


	gob.renderPal = function(r,g,b,a,x,y) {

		gob.shaders.palShader.uniforms.u_PalCol.value.x = r;
		gob.shaders.palShader.uniforms.u_PalCol.value.y = g;
		gob.shaders.palShader.uniforms.u_PalCol.value.z = b;
		gob.shaders.palShader.uniforms.u_PalCol.value.w = a;
		
		gob.shaders.palShader.uniforms.u_PalCoords.value.x = x;
		gob.shaders.palShader.uniforms.u_PalCoords.value.y = y;


		gob.renderToTarget(
			"palShader",
			gob.renderTargets.palRT,
			[
				
			]
		);

	}


	gob.layoutGUI = function(rootObj, firstRun) {



		



		if (firstRun) {
			meshText.maxInd = 0;
			meshBG.maxInd = 0;
		}

		meshText.curInd = 0;
		meshText.extraInd = 0;
		meshBG.curInd = 0;
		meshBG.extraInd = 0;
		
		g_curGroup = 1;
		
		gob.traceVals = ["x","y","resultWidth","resultHeight", "fillDir"];

		if (firstRun) {
			gob.numberGroups(rootObj,5);
			gob.setGroups(rootObj,0);
			gob.getMinWidths(rootObj);
			gob.setResultWidths(rootObj);
			gob.getMinHeights(rootObj);
		}

		

		gob.addMeshes(rootObj,firstRun);
		gob.traceVals = [];


		gob.zeroOutVerts(rootObj,true,firstRun,meshBG);
		gob.zeroOutVerts(rootObj,false,firstRun,meshText);


	};


	/*
	gob.wf("doTextUpdate", function() {
		gob.drawTextArea(testObj, true, false, meshBG);
		gob.drawTextArea(testObj, false, false, meshText);
		gob.updateBaseRT = true;
	});
	*/

	gob.wf("initFinal", function() {
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


					/*
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
					*/
					curShader.transparent = false;
					
					
					
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

		j$(document).mousemove(function(e){

			var x = ((e.pageX/gob.bufferWidth)-0.5)*2.0;
			var y = -((e.pageY/gob.bufferHeight)-0.5)*2.0;


			/*
			var oldRendering = gob.isRendering;

			var maxval = 0.9;

			gob.isRendering = (x < maxval && x > -maxval && y < maxval && y > -maxval);

			if (oldRendering != gob.isRendering) {
				console.log("gob.isRendering: " + gob.isRendering);
			}
			*/

			gob.isRendering = true;

			//TODO: update rendering on mouse movement instead


			gob.shaders.lightingShader.uniforms.u_MouseCoords.value.x = x;
			gob.shaders.lightingShader.uniforms.u_MouseCoords.value.y = y;

			//gob.shaders.textShader.uniforms.u_MouseCoords.value.x = e.pageX;
			//gob.shaders.textShader.uniforms.u_MouseCoords.value.y = e.pageY;
			//gob.shaders.bgShader.uniforms.u_MouseCoords.value.x = e.pageX;
			//gob.shaders.bgShader.uniforms.u_MouseCoords.value.y = e.pageY;


			if (gob.mouseDown) {

				gob.updateActiveComp(e.pageX);
			}


		});
		j$(document).mousedown(function(e){
			
			var wRatio = e.pageX/gob.bufferWidth;
			var hRatio = e.pageY/gob.bufferHeight;

			var res;

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
				
				res = gob.testHit(gob.mainRoot, e.pageX, e.pageY + gob.maxLayers);

				if (res === 0) {
					res = gob.testHit(gob.mainRoot, e.pageX, e.pageY);
				}

				//console.log(res);

				gob.activeComp = res;


				gob.mouseDown = true;

				//testObj.maxWidth = (e.pageX-testObj.x)/zoom;
				//testObj.maxHeight = (e.pageY-testObj.y)/zoom;

				//(obj, isBG, firstRun, geoBuffer)
				//gob.doTextUpdate();
			}


			
		});

		j$(document).mouseup(function(e){
			
			var wRatio = e.pageX/gob.bufferWidth;
			var hRatio = e.pageY/gob.bufferHeight;

			var res;

			if (gob.debugTex) {
				
			}
			else {
				gob.mouseDown = false;
				gob.lockOn = false;
				gob.updateActiveComp(e.pageX);
			}


			
		});

		j$(document).mousewheel(function(event, delta, deltaX, deltaY) {

			//zoom += deltaY/100.0;

		});



		j$(document).keypress(function(e) {

			var code = (e.keyCode ? e.keyCode : e.which);

			
			if ( code == "a".charCodeAt(0) ) {
				gob.autoUpdate = !gob.autoUpdate;
				console.log("Auto Update: " + gob.autoUpdate);
			}
			
		});

		gob.animate();
	});

	gob.init = function() {

		console.log("-- gob.init() --");

		var i;
		var curStr = "";
		

		gob.traceLevel = 0;
		gob.tracesAtLevel = 0;
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
		// NearestFilter // LinearFilter

		gob.renderTargets.baseRT = new THREE.WebGLRenderTarget( gob.bufferWidth, gob.bufferHeight, {
			minFilter: THREE.LinearFilter, 
			magFilter: THREE.LinearFilter, 
			format: THREE.RGBAFormat
		} );

		gob.renderTargets.idRT = new THREE.WebGLRenderTarget( gob.bufferWidth, gob.bufferHeight, {
			minFilter: THREE.NearestFilter,
			magFilter: THREE.NearestFilter, 
			format: THREE.RGBAFormat
		} );
		gob.renderTargets.extrudeRT = new THREE.WebGLRenderTarget( gob.bufferWidth, gob.bufferHeight, {
			minFilter: THREE.NearestFilter,
			magFilter: THREE.NearestFilter, 
			format: THREE.RGBAFormat
		} );

		gob.renderTargets.layerRT = new THREE.WebGLRenderTarget( gob.bufferWidth, gob.bufferHeight, {
			minFilter: THREE.LinearFilter,
			magFilter: THREE.LinearFilter, 
			format: THREE.RGBAFormat
		} );
		gob.renderTargets.aoRT = new THREE.WebGLRenderTarget( gob.bufferWidth, gob.bufferHeight, {
			minFilter: THREE.LinearFilter,
			magFilter: THREE.LinearFilter, 
			format: THREE.RGBAFormat
		} );

		gob.renderTargets.lightingRT = new THREE.WebGLRenderTarget( gob.bufferWidth, gob.bufferHeight, {
			minFilter: THREE.LinearFilter,
			magFilter: THREE.LinearFilter, 
			format: THREE.RGBAFormat
		} );

		gob.renderTargets.downscaleRT = new THREE.WebGLRenderTarget( gob.bufferWidth/2, gob.bufferHeight/2, {
			minFilter: THREE.NearestFilter,
			magFilter: THREE.NearestFilter, 
			format: THREE.RGBAFormat
		} );


		gob.renderTargets.palRT = new THREE.WebGLRenderTarget( 256, 256, {
			minFilter: THREE.NearestFilter,
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

	gob.zeroOutVerts = function(obj,isBG,firstRun,curMesh) {
		

		var i;
		var j;
		var k;
		var curShader;
		var curShaders;

		if (isBG) {
			curShaders = ["bgShader","bgIdShader"];
		}
		else {
			curShaders = ["textShader","textIdShader"];
		}


		//curMesh.maxInd = curMesh.curInd;
		

		if (firstRun) {
			curMesh.maxInd = curMesh.curInd;
		}
		
		// zero out the remaining vertices
		/*
		if (firstRun) {
			curMesh.maxInd = Math.floor( (curMesh.curInd+curMesh.extraInd) * 2);

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

				for (k = 0; k < curShaders.length; k++) {
					for (j = 0; j < a_DataArr.length; j++) {
						gob.shaders[curShaders[k]].attributes[a_DataArr[j]].value.push(

							new THREE.Vector4( 0, 0, 0, 0 ),
							new THREE.Vector4( 0, 0, 0, 0 ),
							new THREE.Vector4( 0, 0, 0, 0 ),
							new THREE.Vector4( 0, 0, 0, 0 )

						);
					}
					
				}
				

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

				for (k = 0; k < curShaders.length; k++) {
					for (j = 0; j < a_DataArr.length; j++) {
						curShader = curShaders[k];
						gob.shaders[curShader].attributes[a_DataArr[j]].value[curInd0].set( 0, 0, 0, 0 );
						gob.shaders[curShader].attributes[a_DataArr[j]].value[curInd1].set( 0, 0, 0, 0 );
						gob.shaders[curShader].attributes[a_DataArr[j]].value[curInd2].set( 0, 0, 0, 0 );
						gob.shaders[curShader].attributes[a_DataArr[j]].value[curInd3].set( 0, 0, 0, 0 );
					}
				}

			}
		}

		*/

		
		



		curMesh.geometry.verticesNeedUpdate = true;
		curMesh.geometry.elementsNeedUpdate = true;
		curMesh.geometry.uvsNeedUpdate = true;

		for (i = 0; i < curShaders.length; i++) {
			for (j = 0; j < a_DataArr.length; j++) {
				gob.shaders[curShaders[i]].attributes[ a_DataArr[j] ].needsUpdate = true;
			}
		}

		//curMesh.geometry.morphTargetsNeedUpdate = true;
		//curMesh.geometry.normalsNeedUpdate = true;
		//curMesh.geometry.colorsNeedUpdate = true;
		//curMesh.geometry.tangentsNeedUpdate = true;
	}
	
	gob.drawTextArea = function(obj, isBG, firstRun, curMesh, calcHeight) {

		var curShaders;
		var curShader;
		var str = obj.str;
		var x = obj.x;
		var y = obj.y;
		var scale = obj.scale;


		obj.maxWidth = obj.resultWidth;
		obj.maxHeight = obj.resultHeight;

		var pMaxW = obj.maxWidth;
		var pMaxH = obj.maxHeight;//maxLines*gob.curFont.metrics.height;
		//var curFont = obj.font;

		var xOff;
		var yOff;

		var totPadding = obj.padding + obj.border;


		var maxW = (pMaxW - totPadding*2.0)/scale;
		var maxH = (pMaxH - totPadding*2.0)/scale;
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

		var hTextAlign = obj.hTextAlign ? obj.hTextAlign:enums.align.left;
		var vTextAlign = obj.vTextAlign ? obj.vTextAlign:enums.align.top;
		var tempWordStart;

		var curInd0;
		var curInd1;
		var curInd2;
		var curInd3;
		var curIndDiv4;

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

		while ( (finalLineArr.length > 0) && (finalLineArr.last().lineWidth == 0) ) {
			finalLineArr.pop();
		}

		while ( (finalLineArr.length > 0) && (finalLineArr[0].lineWidth == 0) ) {
			finalLineArr.shift();
		}



		if (calcHeight) {
			return Math.min(finalLineArr.length, obj.maxLines)*gob.curFont.metrics.height*scale;
		}
		else {


			atArr = atArrs[0];
			atArr[0] =  obj.maxWidth;  atArr[1] = obj.maxHeight;  atArr[2] = obj.groupId;  atArr[3] = 0.0;
			atArr[4] =  obj.maxWidth;  atArr[5] = obj.maxHeight;  atArr[6] = obj.groupId;  atArr[7] = 0.0;
			atArr[8] =  obj.maxWidth;  atArr[9] = obj.maxHeight;  atArr[10] = obj.groupId; atArr[11] = 0.0;
			atArr[12] = obj.maxWidth; atArr[13] = obj.maxHeight; atArr[14] = obj.groupId; atArr[15] = 0.0;

			atArr = atArrs[1];
			atArr[0] =  obj.curValue;  atArr[1] = obj.baseDepth;  atArr[2] =  0.0;  atArr[3] = 0.0;
			atArr[4] =  obj.curValue;  atArr[5] = obj.baseDepth;  atArr[6] =  0.0;  atArr[7] = 0.0;
			atArr[8] =  obj.curValue;  atArr[9] = obj.baseDepth;  atArr[10] = 0.0;  atArr[11] = 0.0;
			atArr[12] = obj.curValue; atArr[13] = obj.baseDepth;  atArr[14] =  0.0; atArr[15] = 0.0;


			if (isBG && obj.drawBG) {
				curShaders = ["bgShader","bgIdShader"];

				
				
				gob.drawString(obj,true, "", x, y, 0, 0, firstRun, curMesh);
			}
			else {
				curShaders = ["textShader","textIdShader"];
				
				/*
				atArr = atArrs[0];
				atArr[0] = 0.0;  atArr[1] = 0.0;  atArr[2] = obj.groupId;  atArr[3] = 0.0;
				atArr[4] = 0.0;  atArr[5] = 0.0;  atArr[6] = obj.groupId;  atArr[7] = 0.0;
				atArr[8] = 0.0;  atArr[9] = 0.0;  atArr[10] = obj.groupId; atArr[11] = 0.0;
				atArr[12] = 0.0; atArr[13] = 0.0; atArr[14] = obj.groupId; atArr[15] = 0.0;

				atArr = atArrs[1];
				atArr[0] =  0.0;  atArr[1] = 0.0;  atArr[2] =  0.0;  atArr[3] = 0.0;
				atArr[4] =  0.0;  atArr[5] = 0.0;  atArr[6] =  0.0;  atArr[7] = 0.0;
				atArr[8] =  0.0;  atArr[9] = 0.0;  atArr[10] = 0.0; atArr[11] = 0.0;
				atArr[12] = 0.0; atArr[13] = 0.0; atArr[14] =  0.0; atArr[15] = 0.0;
				*/

				
				for (i = 0; i < finalLineArr.length; i++) {
					yOff = i*gob.curFont.metrics.height  + gob.curFont.metrics.descender;//ascender + gob.curFont.metrics.descender;
					switch (hTextAlign) {
						case enums.align.left:
							xOff = 0;
						break;
						case enums.align.center:
							xOff = (maxW-finalLineArr[i].lineWidth)/(2);
						break;
						case enums.align.right:
							xOff = (maxW-finalLineArr[i].lineWidth);
						break;
					}
					

					//if ( (i+1)*gob.curFont.metrics.ascender - gob.curFont.metrics.descender < maxH) {
					if ( i < obj.maxLines )
					{	
						gob.drawString(obj,false, finalLineArr[i].lineStr, x, y, xOff, yOff, firstRun, curMesh);
					}
					else {
						curMesh.extraInd += finalLineArr[i].lineStr.length*4;
					}
				}
			}

		}


	}

	gob.drawString = function(obj, isBG, str, xBase, yBase, xOff, yOff, firstRun, curMesh) {
		var curShader;
		var curShaders;
		var scale = obj.scale;
		var isRect = obj.isRect;
		//var curFont = obj.font;

		var i;
		var j;
		var k;
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

		var totPadding = obj.padding + obj.border;

		if (isBG) {
			curShaders = ["bgShader","bgIdShader"];
			strln = 1;
		}
		else {
			curShaders = ["textShader","textIdShader"];
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

				vx = (xBase + (curX + curChar.ox)*scale) + totPadding;
				vy = (yBase + (curY + gob.curFont.metrics.height - (curChar.oy) )*scale) + totPadding;
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

				for (j = 0; j < curShaders.length; j++) {

					for (k = 0; k < a_DataArr.length; k++) {
						atArr = atArrs[k];
						gob.shaders[curShaders[j]].attributes[a_DataArr[k]].value.push(

							new THREE.Vector4( atArr[0], atArr[1], atArr[2], atArr[3] ),
							new THREE.Vector4( atArr[4], atArr[5], atArr[6], atArr[7] ),
							new THREE.Vector4( atArr[8], atArr[9], atArr[10], atArr[11] ),
							new THREE.Vector4( atArr[12], atArr[13], atArr[14], atArr[15] )

						);
					}

					
				}
				

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

					for (j = 0; j < curShaders.length; j++) {
						curShader = curShaders[j];
						for (k = 0; k < a_DataArr.length; k++) {
							atArr = atArrs[k];
							gob.shaders[curShader].attributes[a_DataArr[k]].value[curInd0].set( atArr[0], atArr[1], atArr[2], atArr[3] );
							gob.shaders[curShader].attributes[a_DataArr[k]].value[curInd1].set( atArr[4], atArr[5], atArr[6], atArr[7] );
							gob.shaders[curShader].attributes[a_DataArr[k]].value[curInd2].set( atArr[8], atArr[9], atArr[10], atArr[11] );
							gob.shaders[curShader].attributes[a_DataArr[k]].value[curInd3].set( atArr[12], atArr[13], atArr[14], atArr[15] );
						}
						
					}

					

					//curMesh.geometry.faces[ curIndDiv4 ].set(curInd0, curInd1, curInd2, curInd3);
				}
				else {
					gob.doError("Exceeded Buffer Length (" + curInd0 + " of " + curMesh.maxInd + ")");
				}

				


			}

			curMesh.curInd += 4;

		}

	}

	gob.onWindowResize = function( event ) {

		var i;
		
		
		for (i = 0; i < gob.shaderNames.length; i++) {
			gob.shaders[gob.shaderNames[i]].uniforms.u_Resolution.value.x = gob.bufferWidth;//window.innerWidth;
			gob.shaders[gob.shaderNames[i]].uniforms.u_Resolution.value.y = gob.bufferHeight;//window.innerHeight;
		}

		gob.shaders.downscaleShader.uniforms.u_TexResolution.value.x = Math.floor(gob.bufferWidth/2);
		gob.shaders.downscaleShader.uniforms.u_TexResolution.value.y = Math.floor(gob.bufferHeight/2);
		gob.shaders.upscaleShader.uniforms.u_TexResolution.value.x = Math.floor(gob.bufferWidth/2);
		gob.shaders.upscaleShader.uniforms.u_TexResolution.value.y = Math.floor(gob.bufferHeight/2);

		gob.shaders.bgShader.uniforms.u_MaxLayers.value = gob.maxLayers/255.0;
		gob.shaders.textShader.uniforms.u_MaxLayers.value = gob.maxLayers/255.0;

		//renderer.setClearColorHex( 0xffffff, 1 );
		renderer.setSize( gob.bufferWidth, gob.bufferHeight );


	}

	gob.animate = function() {

		var i;

		requestAnimationFrame( gob.animate );

		if (gob.isRendering || gob.autoUpdate || gob.mouseDown) {
			
			for (i = 0; i < gob.shaderNames.length; i++) {
				gob.shaders[gob.shaderNames[i]].uniforms.u_Zoom.value = zoom;
			}

			gob.shaders.bgShader.uniforms.u_BorderRad.value = gob.styleSheets.defContH.border;
			gob.shaders.bgIdShader.uniforms.u_BorderRad.value = gob.styleSheets.defContH.border;



			if (gob.debugTex) {
				gob.debugTexture();
			}
			else {

				if (gob.updateBaseRT) {


					//gl.disable(gl.BLEND);
					//gl.enable(gl.DEPTH_TEST);

					gob.updateBaseRT = false;
					

					meshText.setMaterial(gob.materials.textShader);
					meshBG.setMaterial(gob.materials.bgShader);
					renderer.render( scene, camera, gob.renderTargets.baseRT, true );


					meshText.setMaterial(gob.materials.textIdShader);
					meshBG.setMaterial(gob.materials.bgIdShader);
					renderer.render( scene, camera, gob.renderTargets.idRT, true );

					
					gob.renderLayers();


					gob.renderToTarget(
						"extrudeShader",
						gob.renderTargets.extrudeRT,
						[
							gob.renderTargets.layerRT,
							gob.renderTargets.idRT
						]
					);



					if (gob.mouseDown) {
						
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
							"aoHighShader",
							gob.renderTargets.aoRT,
							[
								gob.renderTargets.layerRT
							]
						);
					}
					

					

				}
				else {
					
					gob.shaders.lightingShader.uniforms.u_Time.value = performance.now();

					gob.renderPal(1.0,1.0,1.0,1.0, 1.0,1.0);

					gob.renderToTarget(
						"lightingShader",
						undefined,//gob.renderTargets.lightingRT,
						[
							gob.renderTargets.aoRT,
							gob.renderTargets.layerRT,
							gob.renderTargets.extrudeRT,
							gob.renderTargets.palRT
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


		gob.isRendering = false;

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


	gob.wf("setCurFont", function(fName) {
		gob.curFont = fName;
		gob.shaders.textShader.uniforms.u_Texture0.value = gob.curFont.heightRT;
		gob.shaders.textShader.uniforms.u_Texture1.value = gob.curFont.normHalfRT;

		gob.shaders.textIdShader.uniforms.u_Texture0.value = gob.curFont.heightRT;
		gob.shaders.textIdShader.uniforms.u_Texture1.value = gob.curFont.normHalfRT;


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

	});

	gob.wf("initScene", function() {

		var i;

		container = document.getElementById( 'container' );
		camera = new THREE.Camera();
		camera.position.z = 1;
		scene = new THREE.Scene();
		
		var gbText = new THREE.Geometry();
		gbText.dynamic = true;
		meshText = new THREE.Mesh( gbText, gob.materials.textShader);


		var gbBG = new THREE.Geometry();
		gbBG.dynamic = true;
		meshBG = new THREE.Mesh( gbBG, gob.materials.bgShader);
		
		




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
			curh = 2.0*i/gob.bufferHeight;
			curd = i/255.0;

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
		

		gob.styleSheets.defContH = {
			str: 			"",
			drawBG: 		true,
			scale: 			0.5,
			//font: 			gob.curFont,
			hTextAlign:		enums.align.left,
			vTextAlign:		enums.align.top,

			//x: 				50,
			//y: 				50,
			//maxWidth: 		700,
			//maxHeight: 		550,

			//scrollx: 0,
			//scrolly: 0,

			curValue: 		0.0,

			groupId: 		0,

			isGroup: 		false,

			fillDir: 		enums.fillDir.horizontal,
			fillRatio: 		1,
			maxLines: 		3,
			fitText: 		true,
			itemsPerLine: 	0,

			cornerRad: 		8,
			margin: 		0,
			border: 		8,
			padding: 		8,
			
			baseDepth: 		5
		};

		
		gob.styleSheets.defContV = combineObjs([
			gob.styleSheets.defContH,
			{
				fillDir: 		enums.fillDir.vertical,
			}
		]);
		


		gob.mainRoot = createNode( 


			[
				gob.styleSheets.defContH,
				{
					resultWidth:gob.bufferWidth,
					resultHeight:gob.bufferHeight,
					x:0,
					y:0
					//,str:"Lorem ipsum dolor sit amet, pro nostrum ullamcorper at\nLorem ipsum dolor sit amet, pro nostrum ullamcorper at"
				}
			],

			
			[
				createNode( gob.styleSheets.defContV, [
					createNode( [gob.styleSheets.defContH,{isGroup:true,str:"Tg|ST g|T 456"}], []),
					createNode( [gob.styleSheets.defContH,{isGroup:true,str:"test"}], []),
					createNode( [gob.styleSheets.defContH,{isGroup:true,str:"test"}], [])
				]),
				createNode( gob.styleSheets.defContV, [
					createNode( [gob.styleSheets.defContH,{isGroup:true,str:"test 12"}], []),
					createNode( [gob.styleSheets.defContH,{isGroup:true,str:"test"}], []),
				]),
				createNode( gob.styleSheets.defContV, [
					createNode( [gob.styleSheets.defContH,{isGroup:true,str:"test abc"}], []),
					createNode( [gob.styleSheets.defContH,{isGroup:true,str:"test xyz"}], [])
				])
			]
		);

		gob.layoutGUI(gob.mainRoot,true);

		
		scene.add( meshText );
		scene.add( meshBG );
		
		
		container.appendChild( renderer.domElement );
		stats = new Stats();
		stats.domElement.style.position = 'absolute';
		stats.domElement.style.bottom = '0px';
		//container.appendChild( stats.domElement );
		gob.onWindowResize();
		window.addEventListener( 'resize', gob.onWindowResize, false );


	});

	gob.wf("setFont", function(val) {
		//testObj.scale = val;
		gob.setCurFont(val);
		//gob.doTextUpdate();
		
	});

	gob.wf("getKernMap", function(fontObj) {
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

	});

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
	};


	
	j$(document).ready(function(){
		gob.init();
	})
	



});