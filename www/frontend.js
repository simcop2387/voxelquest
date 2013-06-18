var TRACE_ON = true;
var gob;

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

	var container, stats;
	var camera, scene, renderer;
	var uniforms, material, mesh;
	var mouseX = 0, mouseY = 0, lat = 0, lon = 0, phy = 0, theta = 0;
	var windowHalfX = window.innerWidth / 2;
	var windowHalfY = window.innerHeight / 2;
	var shaderNames = ["default"];
	var shaders = {};
	

	var drawString = function(str) {
		var cf = g_fonts["arial_black_regular_48.png"];
		var i;
		var curChar;


		for (i = 0; i < str.length; i++) {
			curChar = str.charAt(i);
		}


	}

	var onWindowResize = function( event ) {

		uniforms.resolution.value.x = window.innerWidth;
		uniforms.resolution.value.y = window.innerHeight;
		renderer.setSize( window.innerWidth, window.innerHeight );

	}

	var animate = function() {

		requestAnimationFrame( animate );
		render();
		stats.update();

	}

	var render = function() {

		uniforms.time.value += 0.05;
		//uniforms.basepos.value.x = Math.abs(Math.sin(uniforms.time.value)*0.5);
		renderer.render( scene, camera )

	}

	var init = function() {

		container = document.getElementById( 'container' );
		camera = new THREE.Camera();
		camera.position.z = 1;
		scene = new THREE.Scene();

		uniforms = {
			time: { type: "f", value: 1.0 },
			resolution: { type: "v2", value: new THREE.Vector2() },
			basepos: { type: "v2", value: new THREE.Vector2() }
		};

		material = new THREE.ShaderMaterial( {

			side: THREE.DoubleSide,
			uniforms: uniforms,
			vertexShader: shaders["default"].vert,
			fragmentShader: shaders["default"].frag

		} );

		//mesh = new THREE.Mesh( new THREE.PlaneGeometry( 1.0,1.0 ), material );
		mesh = new THREE.Mesh( new THREE.PlaneGeometry( 0.5,0.5 ), material );
		mesh.position = new THREE.Vector3(0, 0, 0);

		scene.add( mesh );

		renderer = new THREE.WebGLRenderer();
		container.appendChild( renderer.domElement );

		stats = new Stats();
		stats.domElement.style.position = 'absolute';
		stats.domElement.style.top = '0px';
		//container.appendChild( stats.domElement );

		onWindowResize();

		window.addEventListener( 'resize', onWindowResize, false );


	}

	var preload = function() {

		var i;

		for (i = 0; i < shaderNames.length; i++) {


			jQuery.ajax({
				url: './shaders/'+shaderNames[i]+'.c',
				success: function(result) {

					var splitText = result.split("$");
					shaders[shaderNames[i]] = {};
					shaders[shaderNames[i]].vert = splitText[0] + splitText[1];
					shaders[shaderNames[i]].frag = splitText[0] + splitText[2];



					//console.log(result);
				},
				async:   false,
				dataType:"text"
			});

		}

		init();
		animate();
		
	}

	preload();


	





});