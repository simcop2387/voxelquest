//uniform sampler3D Texture0; //volume texture
uniform sampler2D Texture0;

//uniform sampler2D Texture0;
//uniform sampler3D Texture1;
varying vec3 TexCoord0;
varying vec3 TexCoord1;

uniform vec3 worldMinVisInPixels;
uniform vec3 worldMaxVisInPixels;
uniform vec3 worldMinBufInPixels;
uniform vec3 worldMaxBufInPixels;

uniform vec4 scaleAndOffset;

//uniform vec3 offsetInPagesLocal;

uniform float curLayer;
uniform float pageDepth;
//uniform float directPass;
uniform float bufferMult;
uniform float visPageSizeInPixels;

//const int iNumRaySteps = 2;
//const float fNumRaySteps = 2.0;

//uniform float slicesPerPitch;
uniform float volumePitch;
//varying float pitch;// = 256.0;
//varying float pitchM1;// = pitch-1.0;



$

void main() {
    
    TexCoord0 = gl_MultiTexCoord0.xyz;
    TexCoord1 = gl_MultiTexCoord1.xyz;

    vec4 newPos = gl_Vertex;


    newPos.x *= scaleAndOffset.x;
    newPos.y *= scaleAndOffset.y;
    newPos.x += scaleAndOffset.z;
    newPos.y += scaleAndOffset.w;

    //newPos.z = pageDepth;

    gl_Position = newPos;

}

$









// vec4 sampleAtPoint(vec3 point) {
//     vec2 texc;
//     vec3 newPoint = point/bufferMult + (1.0-1.0/bufferMult)/2.0;
//     vec4 texVal = texture3D(Texture0, newPoint);
//     //texVal.r *= texVal.r;
//     return texVal;
// }





vec4 sampleAtPoint(vec3 point) {
    vec2 texc;
    vec3 newPoint = point/bufferMult + (1.0-1.0/bufferMult)/2.0;

    newPoint.y = clamp(newPoint.y,0.5/volumePitch,(volumePitch-0.5)/volumePitch);

    texc.x = newPoint.x;
    texc.y = newPoint.y/volumePitch + floor(newPoint.z*volumePitch)/volumePitch;

    return texture2D(Texture0, texc.xy);
}




// vec4 sampleAtPoint(vec3 point) {
//     vec2 texc;
//     vec3 newPoint = point/bufferMult + (1.0-1.0/bufferMult)/2.0;
//     vec4 t1;
//     vec4 t2;

//     newPoint.y = clamp(newPoint.y,0.5/volumePitch,(volumePitch-0.5)/volumePitch);
    
//     float vp2 = volumePitch*volumePitch;
//     float zfloor = floor(newPoint.z*volumePitch); // 0 to 64
//     float zmod = (newPoint.z*volumePitch - zfloor); // 0 to 1

//     texc.x = newPoint.x;
//     texc.y = newPoint.y/volumePitch + (zfloor)/volumePitch;
//     //texc.y = floor(texc.y*vp2)/vp2 + 0.5/vp2;

//     texc.y -= float(zmod < 0.5)/volumePitch;
//     //texc.y = floor(texc.y*vp2)/vp2 + 0.5/vp2;
//     t1 = texture2D(Texture0, texc.xy);
//     texc.y += 1.0/volumePitch;
//     //texc.y = floor(texc.y*vp2)/vp2 + 0.5/vp2;
//     t2 = texture2D(Texture0, texc.xy);

//     return mix(t1,t2,zmod);

//     return texture3D(Texture0, newPoint);
// }

// vec4 sampleAtPointN(vec3 point) {
//     vec2 texc;
//     vec3 newPoint = point/bufferMult + (1.0-1.0/bufferMult)/2.0;

//     return texture3D(Texture0, newPoint);

//     // float vp2 = volumePitch*volumePitch;
//     // newPoint.y = clamp(newPoint.y,0.5/volumePitch,(volumePitch-0.5)/volumePitch);

//     // texc.x = floor(newPoint.x*volumePitch)/volumePitch + 0.5/volumePitch;
//     // texc.y = newPoint.y/volumePitch + floor(newPoint.z*volumePitch)/volumePitch;
//     // texc.y = floor(texc.y*vp2)/vp2 + 0.5/vp2;


//     // return texture2D(Texture0, texc.xy);
    

//     //return texture3D(Texture0, newPoint);
// }


// vec4 sampleAtPoint(vec3 point) {
//     vec2 texc;

//     float pitch = slicesPerPitch*slicesPerPitch;
//     vec3 newPoint = point/bufferMult + (1.0-1.0/bufferMult)/2.0;

//     newPoint = clamp(newPoint,)
    
//     float bval = newPoint.b*pitch;
//     float xval = floor(mod(bval, slicesPerPitch))/slicesPerPitch;
//     float yval = floor(bval/slicesPerPitch)/slicesPerPitch;
    
//     texc.x = newPoint.r/(slicesPerPitch) + xval;
//     texc.y = newPoint.g/(slicesPerPitch) + yval;
//     return texture2D(Texture0, texc.xy);
    

//     //return texture3D(Texture0, newPoint);
// }


/*vec4 sampleAtPoint(vec3 point) {
    vec2 texc;

    float pitch = slicesPerPitch*slicesPerPitch;
    float pitchM1 = pitch-1.0;

    vec3 newPoint = point/bufferMult + (1.0-1.0/bufferMult)/2.0;
    
    vec3 curFace = (newPoint.rgb*pitchM1+0.5)/pitch;
    float bval = curFace.b*pitchM1;
    float xval = floor(mod(bval, slicesPerPitch))/slicesPerPitch;
    float yval = floor(bval/slicesPerPitch)/slicesPerPitch;
    
    texc.x = curFace.r/(slicesPerPitch) + xval;
    texc.y = curFace.g/(slicesPerPitch) + yval;
    return texture2D(Texture0, texc.xy);
    

    //return texture3D(Texture0, newPoint);
}
*/

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

vec4 getAO(vec3 tp, vec4 curSamp, vec3 wp) {
    int i;
    int j;
    float fi = 0.0;
    float fj = 0.0;
    int a, b, c;
    
    

    vec3 offVal;
    vec3 offValAbs;

    float len;
    

    float aoVal = 0.0;
    float totalAO = 0.0;

    float notZero;
    float fLerp = 0.0;


    vec4 res;

    int maxRad;
    float tsize = visPageSizeInPixels;
    float pixMod = 1.0/(tsize);
    vec3 testPoint = tp + pixMod; //bufferMult

    float fMaxRad;
    float curPower;

    float rval;
    float rval2;

    vec3 norm = vec3(0.0,0.0,0.0);
    vec3 norm2 = vec3(0.0,0.0,0.0);

    int rad;
    int radStep;
    float theta = 0.0;
    float phi = 0.0;
    float frad = float(radStep);

    float totSteps = 0.0;
    float totHits = 0.0;

    float allHits = 0.0;

    float ovLen = 0.0;
    float rvMix = 0.0;

    float twoPI = 6.2831853;
    float PI = 3.14159;

    float isCurLayer = 0.0;

    float isAir = 0.0;
    //


    //float sval = abs(sin(wp.y/30.0));

    //float isRock = float( curSamp.r + curSamp.g + curSamp.b > 0.0);


    rad = 1;
    radStep = 1;

    for (i = 0; i < 2; i++) {

        for (c = -rad; c <= rad; c+=radStep) {
            for (b = -rad; b <= rad; b+=radStep) {
                for (a = -rad; a <= rad; a+=radStep) {
                    
                    offVal.x = float(a);
                    offVal.y = float(b);
                    offVal.z = float(c);

                    //offVal = normalize(offVal);
                    //offValAbs = offVal*frad/tsize;

                    //ovLen = 1.0/length(offVal);

                    res = sampleAtPoint(offVal/tsize + testPoint);


                    isCurLayer = float(res.r == curLayer);
                    isAir = float(res.a*isCurLayer == 0.0);

                    
                    allHits += isAir;

                    if (curSamp.g == 0.0) {
                        rval = isAir;
                    }
                    else {
                        rval = float( (res.g != curSamp.g) || (isAir == 1.0) );
                    }

                    

                    

                    //rval2 = float( abs(res.r-curSamp.r) + abs(res.g-curSamp.g) + abs(res.b-curSamp.b) != 0.0 );
                    //rvMix = mix(rval,rval2,0.5);//mix(rval,rval2, isRock);//abs(sin(wp.y)) );//isRock
                    //norm += rvMix*(offVal);
                    norm += rval*offVal;

                    totHits += rval;
                    //totSteps += 1.0;
                    
                }
            }
        }

        if (totHits == 0.0) {
            break;
        }
        else {
            rad = 12;
            radStep = 4;
        }

        
    }

    




    // for (j = 1; j < 7; j++) {
    //     for (i = 0; i < 32; i++) {

    //         fi = float(i);
    //         fLerp = fi/32.0;

    //         theta = fLerp*PI;
    //         phi = fLerp*twoPI*8.0;
    //         frad = float(j)*2.0/tsize;

    //         offVal.x = sin(theta)*cos(phi);
    //         offVal.y = sin(theta)*sin(phi);
    //         offVal.z = cos(theta);

    //         offVal *= frad;
    //         offVal += 1.0/tsize;


    //         res = sampleAtPoint(offVal+testPoint); //+norm*2.0/(tsize)
    //         rval = float(res.a != 0.0);
    //         norm += rval*offVal;
    //         totHits += rval;

    //     }
    // }


    
    if (totHits == 0.0) {
        norm = vec3(0.0,0.0,0.0);//norm2/totSteps;

        if (tp.x >= tp.y) {

            if (tp.x >= tp.z) {
                norm.x = 1.0;
            }
            else {
                norm.z = 1.0;
            }
        }
        else {
            if (tp.y >= tp.z) {
                norm.y = 1.0;
            }
            else {
                norm.z = 1.0;
            }
        }

        
        // if (testPoint.x == 1.0) {
        //     norm.x = 1.0;
        // }
        // if (testPoint.y == 1.0) {
        //     norm.y = 1.0;
        // }
        // if (testPoint.z == 1.0) {
        //     norm.z = 1.0;
        // }
        

        aoVal = 0.0;
    }
    else {

        aoVal = float(allHits > 0.0);

        // for (j = 1; j < 6; j++) {
        //     fj = float(j);
        //     for (i = 0; i < 32; i++) {

        //         fi = float(i);
        //         fLerp = fi/32.0;

        //         theta = fLerp*PI;
        //         phi = fLerp*twoPI*8.0;
        //         frad = fj*4.0/tsize;

        //         offVal.x = sin(theta)*cos(phi);
        //         offVal.y = sin(theta)*sin(phi);
        //         offVal.z = cos(theta);

        //         offVal *= frad;

        //         offVal += 4.0/tsize;
        //         //offVal += sin( (fi+fj)/8.0 )*32.0/tsize;

        //         //offVal = offVal*16.0;

        //         //offValAbs = abs(offVal);

        //         //notZero = float( (offValAbs.x + offValAbs.y + offValAbs.z) > 0.0);
        //         //offVal = normalize(offVal);
        //         //offVal *= frad/tsize;
        //         res = sampleAtPoint(offVal+testPoint); //+norm*2.0/(tsize)
        //         aoVal += float(res.a>0.0);//*notZero;// *curPower;//frad;
        //         totalAO += 1.0;// *curPower;//frad*notZero;

        //     }
        // }

        //aoVal = clamp( 1.0 - aoVal/totalAO, 1.0/255.0, 1.0);
    }
    


    norm = normalize(norm);
    norm = (norm + 1.0)/2.0;





    return vec4(norm,aoVal); //totHits/totSteps
}


int intMod(int lhs, int rhs) {
    return lhs - ( (lhs/rhs)*rhs );
}

vec2 pack16(float num) {

    int iz = int(num);
    int ir = intMod(iz,256);
    int ig = (iz)/256;

    vec2 res;

    res.r = float(ir)/255.0;
    res.g = float(ig)/255.0;

    return res;

}

float unpack16(vec2 num) {
    return num.r*255.0 + num.g*65280.0;
}

/*
void main() {
    
    gl_FragData[0] = vec4(1.0);
    gl_FragData[1] = vec4(1.0);
}
*/

void main() {

    int i = 0;
    int j = 0;

    float fi;
    float fi2;
    float fiRes;
    float fj;
    float fLerp = 0.0;
    
    vec2 newCoords;
    vec3 front = TexCoord1.xyz;
    vec3 back = TexCoord0.xyz;

    
    vec4 blackCol = vec4(0.0,0.0,0.0,0.0);
    vec4 samp = vec4(0.0,0.0,0.0,0.0);
    vec4 sampNearest = vec4(0.0,0.0,0.0,0.0);
    
    float curDis = 0.0;
    int iCurDis = 0;  

    vec3 curPos = vec3(0.0,0.0,0.0);
    //vec3 lastGoodPos = vec3(0.0,0.0,0.0);
 
    ///sqrt(3.0)


    curDis = floor(distance(front,back)*visPageSizeInPixels*2.0); //visPageSizeInPixels
    iCurDis = int(curDis);

    float q;

    for (i = 0; i < iCurDis; i++) {
        fi = float(i);
        fLerp = fi/curDis;

        curPos = mix(front,back,fLerp);

        samp = sampleAtPoint(curPos);


        if ( (samp.a > 0.0)&&(samp.r == curLayer) ) {

            break;
        }

        //lastGoodPos = curPos;
    }

    if ( i == iCurDis ) {
        discard;
    }

/*
    fi = float(i-1);
    fi2 = float(i+1);
    for (j = 0; j < 20; j++) {
        fj = float(j);
        
        fLerp = mix(fi,fi2,fj/19.0)/curDis;

        curPos = mix(front,back,fLerp);

        samp = sampleAtPoint(curPos);

        if (samp.a >= 0.0) {
            break;
        }
    }
    */

    sampNearest = sampleAtPoint(curPos);

    vec3 worldPos;

    worldPos.x = mix(worldMinVisInPixels.x, worldMaxVisInPixels.x, curPos.x);
    worldPos.y = mix(worldMinVisInPixels.y, worldMaxVisInPixels.y, curPos.y);
    worldPos.z = mix(worldMinVisInPixels.z, worldMaxVisInPixels.z, curPos.z);


    vec2 heightVals = pack16(worldPos.z);

    vec4 normAO = blackCol;
    vec4 heightMat = blackCol;

    
    
    heightMat = vec4(heightVals.rg, sampNearest.ba);
    normAO = getAO(curPos, samp, worldPos);
 
    gl_FragData[0] = heightMat;
    gl_FragData[1] = normAO;
    
    // if (directPass == 1.0) {
    //     gl_FragData[0] = samp;
    //     gl_FragData[1] = samp;
    // }
    // else {
        
    // }
    

}
