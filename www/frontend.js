var gob;

$(function() {

	gob = {
		traceLevel: 0,
		popCount: 0,
		traceArr: [],
		myInterval: null,
		pushTrace: function() {
			gob.traceLevel++;
			gob.popCount = 0;
		},
		popTrace: function() {
			gob.popCount++;
			gob.traceLevel--;

			if (gob.popCount >= 2) {
				gob.doTrace("END");
			}
		},
		doTrace:function(traceStr) {
			console.log(gob.getTracePrefix() + traceStr);
		},
		getTracePrefix: function() {
			return gob.traceArr[gob.traceLevel];
		}

	};

	var wf = function(varName, func) {
		gob[varName] = function() {
			gob.pushTrace();
			console.log(varName + "()");
			func();
			gob.popTrace();
		}

		return gob[varName];
	}


	gob.init = function() {
		var i;
		var curStr = "";

		gob.traceLevel = 0;
		gob.popCount = 0;
		gob.traceArr = [];

		for (i = 0; i < 100; i++) {
			gob.traceArr.push(curStr);
			curStr += "|  ";
		}

		if (window.WebSocket || window.MozWebSocket) {} else {
			gob.doTrace("Browser does not support web sockets")
			return;
		}

		gob.connection = new WebSocket('ws://127.0.0.1:9980');
		gob.connection.onopen = wf("connectionOnOpen",function() {
			gob.sendMessage("hahsdfhasdf");
		});
		gob.connection.onerror = wf("connectionOnError",function(error) {
			gob.doTrace(error);
		});
		gob.connection.onmessage = wf("connectionOnMessage",function(message) {
			gob.doTrace(message);
			//var json = JSON.parse(message.data);
		});

		wf("sendMessage",function(msg) {
			gob.connection.send(msg);
		});

		gob.myInterval = setInterval(function() {
			if (gob.connection.readyState !== 1) {
				gob.doTrace("Unable to comminucate with the WebSocket server.");
				clearInterval(gob.myInterval);
			}

		}, 3000);

	}



	gob.init();
	



});