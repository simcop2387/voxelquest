
var gob = {};

gob.updatePalCanv = function() {
    var canvas = document.getElementById('canvas');
    var canvasWidth  = canvas.width;
    var canvasHeight = canvas.height;
    var ctx = canvas.getContext('2d');
    var imageData = ctx.getImageData(0, 0, canvasWidth, canvasHeight);
    var buf = new ArrayBuffer(imageData.data.length);
    var buf8 = new Uint8ClampedArray(buf);
    var data = new Uint32Array(buf);
    var value;
    var o;
    var n;
    var m;
    var lerpVal;


    var cl = new CIELCh(0,0,0);
    var omax;

    var hueVals = [

        10,
        40,
        70,
        110,
        170,
        210,
        250,
        290
    ]

    for (n = 0; n < 256; n++) {
        for (m = 0; m < 256; m++) {



            data[(n) * canvasWidth + m] =
                (255   << 24) |
                (m << 16) |
                (m <<  8) |
                 m;
        }
    }

    for (o = 0; o < 4; o++) {
        for (n = 0; n < 8; n++) {
            for (m = 0; m < 8; m++) {

                omax = o*23.0 + 30.0;
                
                cl.c = omax;
                cl.l = Math.floor(n*75.0/7.0 + (12-o*3));
                cl.h = hueVals[m];

                /*
                if (n < 4) {
                    lerpVal = n/3.0;
                    cl.h = hueVals[m]*lerpVal + (1.0-lerpVal)*40.0;
                }
                else {
                    lerpVal = ((n-4.0)/3.0)*0.5;
                    cl.h = (1.0-lerpVal)*hueVals[m] + 250.0*lerpVal;
                }
                */


                value = cl.toRGB();

                //canvasWidth = 8;

                data[(n+o*8) * canvasWidth + m] =
                    (255   << 24) |
                    (value.r << 16) |
                    (value.g <<  8) |
                     value.b;

            }
        }
    }

    imageData.data.set(buf8);
    ctx.putImageData(imageData, 0, 0);
}

gob.updatePalCanv();