
  ////////////////  RenderVolume  ////////////////  

uniform sampler2D Texture0;
//uniform sampler3D Texture1;
varying vec3 TexCoord0;
varying vec3 TexCoord1;

uniform float curHeight;

//const int iNumRaySteps = 2;
//const float fNumRaySteps = 2.0;

$

void main() {

    TexCoord0 = gl_MultiTexCoord0.xyz;
    TexCoord1 = gl_MultiTexCoord1.xyz;
    gl_Position = gl_Vertex;

}

$

vec4 sampleAtPoint(vec3 point) {
    vec2 texc;

    //get rid of this to sample full volume
    vec3 newPoint = point/2.0 + 0.25;

    vec3 curFace = (newPoint.rgb*255.0+0.5)/256.0;
    float bval = curFace.b*255.0;
    float xval = floor(mod(bval, 16.0))/16.0;
    float yval = floor(bval/16.0)/16.0;
    
    texc.x = curFace.r/(16.0) + xval;
    texc.y = curFace.g/(16.0) + yval;
    return texture2D(Texture0, texc.xy);
}

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

vec4 getAO(vec3 testPoint) {
    int i;
    int a, b, c;
    int rad;
    int radStep;

    vec3 offVal;
    vec3 offValAbs;

    float len;
    float frad;
    

    float aoVal = 0.0;
    float totalAO = 0.0;

    float notZero;



    vec4 res;

    int maxRad;
    float tsize = 255.0;

    float fMaxRad;
    float curPower;

    float rval;

    vec3 norm = vec3(0.0,0.0,0.0);

    rad = 8;
    frad = float(rad);
    radStep = 2;

        for (c = -rad; c <= rad; c+=radStep) {
            for (b = -rad; b <= rad; b+=radStep) {
                for (a = -rad; a <= rad; a+=radStep) {
                    
                    offVal.x = float(a);
                    offVal.y = float(b);
                    offVal.z = float(c);

                    offVal = normalize(offVal);
                    offValAbs = offVal*frad/tsize;

                    res = sampleAtPoint(offValAbs+testPoint);
                    rval = float(res.r <= 0.5);

                    norm += rval*offVal;
                    
                }
            }
        }

    norm = normalize(norm);

    if ((norm.x + norm.y + norm.z) == 0.0) {
        if (testPoint.x > testPoint.y) {
            if (testPoint.z > testPoint.x) {
                norm.z = 1.0;
            }
            else {
                norm.x = 1.0;
            }
        }
        else {
            if (testPoint.z > testPoint.y) {
                norm.z = 1.0;
            }
            else {
                norm.y = 1.0;
            }
        }
    }

    /*
    rad = 32;
    frad = float(rad);
    radStep = rad/8;
    
    //maxRad = 64;
    //fMaxRad = float(maxRad);
    */

    maxRad = 64;

    for (rad = 16; rad < maxRad; rad *= 2) {
        radStep = rad/2;
        frad = float(rad);

        //curPower = 1.0-frad/fMaxRad;

        for (c = -rad; c <= rad; c+=radStep) {
            for (b = -rad; b <= rad; b+=radStep) {
                for (a = -rad; a <= rad; a+=radStep) {
                    
                    offVal.x = float(a);
                    offVal.y = float(b);
                    offVal.z = float(c);

                    //offVal = offVal*16.0;

                    offValAbs = abs(offVal);

                    notZero = float( (offValAbs.x + offValAbs.y + offValAbs.z) > 0.0);
                    offVal = normalize(offVal);
                    offVal *= frad/tsize;
                    res = sampleAtPoint(offVal+testPoint); //+norm*2.0/(tsize)
                    rval = float(res.r > 0.5);
                    aoVal += rval*notZero;// *curPower;//frad;
                    totalAO += notZero;// *curPower;//frad*notZero;
                    
                }
            }
        }
    }

    aoVal = clamp(1.0 - aoVal/totalAO, 0.0, 1.0);

    norm = (norm + 1.0)/2.0;

    return vec4(norm.x,norm.y,norm.z,aoVal);
}

void main() {

    //vec4 tex0 = texture2D(Texture0, TexCoord0.xy); //rg: height, ba: type
    //vec4 tex1 = texture2D(Texture1, TexCoord0.xy); //rgb: norm, a: ambOc


    int i;
    float fLerp;
    vec4 curPos;
    vec2 newCoords;
    vec3 front = TexCoord1.xyz;
    vec3 back = TexCoord0.xyz;
    float fRayDis = floor(distance(front,back)*256.0*4.0);
    int iRayDis = int(fRayDis);
    vec4 blackCol = vec4(0.0,0.0,0.0,0.0);
    vec4 samp;
    vec4 samp2;
    vec2 samp3;

    curPos.a = 1.0;

    
    for (i = 0; i < iRayDis; i++) {
        fLerp = float(i)/fRayDis;

        curPos.xyz = mix(front,back,fLerp);

        //curPos.z = curPos.z + 2.0/255.0;

        samp = sampleAtPoint(curPos.xyz);

        if (samp.r > 0.5) {
            break;
        }
    }

    vec4 normAO;
    vec4 heightMat;
    float res = float(i >= iRayDis);

    if (res >= 1.0) {
        normAO = blackCol;
        heightMat = blackCol;
    }
    else {
        normAO = getAO(curPos.xyz);
        heightMat = vec4(curPos.b, curHeight, 1.0, 1.0);
    }
    
    gl_FragData[0] = heightMat;//vec4(TexCoord1.z/256.0,TexCoord1.z/(256.0*256.0),1.0,1.0);
    gl_FragData[1] = normAO;//gl_FragData[0];

    /*
    vec4 res = sampleAtPoint(front);
    gl_FragData[0] = vec4(0.0,0.0,0.0,1.0);
    gl_FragData[0].rgb = res.rgb;
    */
}
