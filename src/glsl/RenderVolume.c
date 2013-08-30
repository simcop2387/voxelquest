////////////////  RenderVolume  ////////////////  

//uniform sampler3D Texture0; //volume texture
uniform sampler2D Texture0;

//uniform sampler2D Texture0;
//uniform sampler3D Texture1;
varying vec3 TexCoord0;
varying vec3 TexCoord1;

uniform float curHeight;
uniform vec3 worldMin;
uniform vec3 worldMax;

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
    

    //return texture3D(Texture0, newPoint);
}

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

vec4 getAO(vec3 testPoint, vec4 curSamp, vec3 wp) {
    int i;
    int a, b, c;
    

    

    vec3 offVal;
    vec3 offValAbs;

    float len;
    

    float aoVal = 0.0;
    float totalAO = 0.0;

    float notZero;



    vec4 res;

    int maxRad;
    float tsize = 255.0;

    float fMaxRad;
    float curPower;

    float rval;
    float rval2;

    vec3 norm = vec3(0.0,0.0,0.0);
    vec3 norm2 = vec3(0.0,0.0,0.0);

    int rad = 8;
    int radStep = 2;
    float frad = float(radStep);

    float totSteps = 0.0;
    float totHits = 0.0;

    float ovLen = 0.0;

    float rvMix = 0.0;

    //float sval = abs(sin(wp.y/30.0));

    float isRock = float( curSamp.r + curSamp.g + curSamp.b > 0.0);

    for (c = -rad; c <= rad; c+=radStep) {
        for (b = -rad; b <= rad; b+=radStep) {
            for (a = -rad; a <= rad; a+=radStep) {
                
                offVal.x = float(a);
                offVal.y = float(b);
                offVal.z = float(c);

                //offVal = normalize(offVal);
                //offValAbs = offVal*frad/tsize;

                ovLen = 1.0/length(offVal);

                res = sampleAtPoint(offVal/tsize + testPoint);
                rval = float(res.a == 0.0);

                rval2 = float( abs(res.r-curSamp.r) + abs(res.g-curSamp.g) + abs(res.b-curSamp.b) != 0.0 );
                rvMix = mix(rval,rval2, isRock);//abs(sin(wp.y)) );//isRock
                norm += rvMix*(offVal);
                //norm2 += res.a*offVal;

                totHits += rval;
                totSteps += 1.0;
                
            }
        }
    }

    /*
    if (totHits == 0.0) {
        norm = vec3(0.0,0.0,0.0);//norm2/totSteps;

        
        if (testPoint.x == 1.0) {
            norm.x = curSamp.a*10.0;
        }
        if (testPoint.y == 1.0) {
            norm.y = curSamp.a*10.0;
        }
        if (testPoint.z == 1.0) {
            norm.z = curSamp.a*4.0;
        }
        

        aoVal = 0.0;//curSamp.a;
    }
    else {

    }
    */


        maxRad = 64;
        fMaxRad = float(maxRad);

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
                        rval = float(res.a != 0.0);
                        aoVal += rval*notZero;// *curPower;//frad;
                        totalAO += notZero;// *curPower;//frad*notZero;
                        
                    }
                }
            }
        }

        aoVal = clamp(1.0 - aoVal/totalAO, 1.0/255.0, 1.0);
    


    norm = normalize(norm);
    norm = (norm + 1.0)/2.0;





    return vec4(norm,aoVal); //totHits/totSteps
}


void main() {

    int i = 0;
    int j = 0;

    float fi;
    float fLerp = 0.0;
    
    vec2 newCoords;
    vec3 front = TexCoord1.xyz;
    vec3 back = TexCoord0.xyz;

    vec3 worldFront = front;
    vec3 worldBack = back;

    worldFront.x *= worldMax.x;
    worldFront.y *= worldMax.y;
    worldFront.z *= worldMax.z;
    worldBack.x *= worldMin.x;
    worldBack.y *= worldMin.y;
    worldBack.z *= worldMin.z;

    
    
    float curDis = 0.0;
    int iCurDis = 0;
    float fRayDis = floor(distance(front,back)*128.0*sqrt(2.0));
    vec4 blackCol = vec4(0.0,0.0,0.0,0.0);
    vec4 samp = vec4(0.0,0.0,0.0,0.0);
    

    vec3 curPos = vec3(0.0,0.0,0.0);


    curDis = fRayDis;
    iCurDis = int(curDis);

    for (i = 0; i < iCurDis; i++) {
        fi = float(i);
        fLerp = fi/curDis;

        curPos = mix(front,back,fLerp);
        samp = sampleAtPoint(curPos);

        if (samp.a != 0.0) {
            break;
        }
    }

    vec3 worldPos = mix(worldFront,worldBack,fLerp);


    vec4 normAO = blackCol;
    vec4 heightMat = blackCol;

    if ( i == iCurDis ) {
        
    }
    else {
        normAO = getAO(curPos, samp, worldPos);
        heightMat = vec4(curPos.b, curHeight, 1.0, 1.0);

        
    }
    
    gl_FragData[0] = heightMat;
    gl_FragData[1] = normAO;

}
