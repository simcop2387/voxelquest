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
			gob.doTraceTab("MESSAGE: " + message.data);
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


	gob.init();




});