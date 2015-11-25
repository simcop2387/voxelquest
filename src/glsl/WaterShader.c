#version 120

// old combine
uniform sampler2D Texture0;
uniform sampler2D Texture1;

// water combine
uniform sampler2D Texture2;
uniform sampler2D Texture3;

// rendered
uniform sampler2D Texture4;

// rendered blurred
uniform sampler2D Texture5;

// noise fbo
uniform sampler2D Texture6;

// wave fbo
//uniform sampler2D Texture7;

// pal fbo
uniform sampler3D Texture7;

// prelight fbo
uniform sampler2D Texture9;
uniform sampler2D Texture10;
uniform sampler2D Texture11;
uniform sampler2D Texture12;

// geom fbo
// uniform sampler2D Texture13;
// uniform sampler2D Texture14;

varying vec2 TexCoord0;


uniform mat4 modelviewInverse;
uniform float seaLevel;
uniform float FOV;
uniform vec2 clipDist;
uniform float timeOfDay;
uniform float isUnderWater;
uniform float curTime;
uniform vec3 cameraPos;
uniform vec2 bufferDim;
uniform vec3 lookAtVec;


^INCLUDE:MATERIALS^

const float pi = 3.14159;

const int maxEntries = 6;
const int maxEntriesM1 = maxEntries-1;



// vec3 colVecs[4] = vec3[](
//     vec3( 1.0, 1.0, 1.0 ),
//     vec3( 1.0, 0.0, 1.0 ),
//     vec3( 0.0, 1.0, 1.0 ),
//     vec3( 0.0, 1.0, 1.0 )
// );

$

void main() {

    TexCoord0 = gl_MultiTexCoord0.xy;// = ;//TexCoord0 = gl_MultiTexCoord0;
    gl_Position = gl_Vertex;
}

$

vec3 rgb2hsv(vec3 c)
{
    vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
    vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
    vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));

    float d = q.x - min(q.w, q.y);
    float e = 1.0e-10;
    return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
}

vec3 randv(vec2 co) {
    return vec3(
        fract(sin(dot(co.xy ,vec2(12.989, 78.233))) * 43758.8563),
        fract(sin(dot(co.yx ,vec2(93.989, 67.345))) * 24634.6345),
        fract(sin(dot(co.yx ,vec2(43.332, 93.532))) * 56445.2345)
    );
}

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

int intMod(int lhs, int rhs) {
    return lhs - ( (lhs/rhs)*rhs );
}
float unpack16(vec2 num) {
    return num.r*255.0 + num.g*65280.0;
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

vec3 unpackColor(vec2 num, float lightVal)
{
    return texture3D( Texture7, vec3(lightVal, num.r, num.g + 0.5/255.0) ).rgb;
}

const int rad = 4;








vec3 getRay() {
    float aspect = bufferDim.y/bufferDim.x;
    float NEAR = clipDist.x;
    float FAR = clipDist.y;
    float dx = tan(FOV*0.5f)*(TexCoord0.x*2.0-1.0f)/aspect; //gl_FragCoord.x/(bufferDim.x*0.5)
    float dy = tan(FOV*0.5f)*((1.0f-TexCoord0.y)*2.0-1.0f); //gl_FragCoord.y/(bufferDim.y*0.5)
    
    
    dx = -dx;
    
    vec4 p1 = vec4(dx*NEAR,dy*NEAR,NEAR,1.0);
    vec4 p2 = vec4(dx*FAR,dy*FAR,FAR,1.0);
    
    p1 = modelviewInverse*p1;
    p2 = modelviewInverse*p2;

    vec3 ro = p1.xyz;
    
    vec3 rd = normalize(p1.xyz-p2.xyz);
    
    return rd;
}






void main() {

    vec4 tex0 = texture2D(Texture0, TexCoord0.xy);
    vec4 tex1 = texture2D(Texture1, TexCoord0.xy);

    vec4 tex2 = texture2D(Texture2, TexCoord0.xy);
    vec4 tex3 = texture2D(Texture3, TexCoord0.xy);

    vec4 tex4 = texture2D(Texture4, TexCoord0.xy);
    vec4 tex5 = texture2D(Texture5, TexCoord0.xy);
    
    vec4 tex10 = texture2D(Texture10, TexCoord0.xy);
    
    vec4 tex11 = texture2D(Texture11, TexCoord0.xy);
    
    // vec4 tex11 = texture2D(Texture0, TexCoord0.xy);
    // vec4 tex12 = texture2D(Texture1, TexCoord0.xy);
    

    //float tot = float(tex0.r + tex0.g + tex0.b + tex0.a > 0.0);    
    vec4 oneVec = vec4(1.0);
    
    //tex0.w = max(tex0.w,tex11.w);
    
    int i;
    int j;
    // float fi;
    // float fj;
    // vec2 offVec = vec2(0.0);
    // vec4 samp = vec4(0.0);
    
    
    // float counter = 0.0;
    
    // for (j = -rad; j <= rad; j++) {
    //     fj = float(j);
    //     for (i = -rad; i <= rad; i++) {
    //         fi = float(i);
            
    //         offVec = vec2(fi,fj)/bufferDim;
            
    //         samp += texture2D(Texture0, TexCoord0.xy+offVec);
            
    //         counter += 1.0;
    //     }
    // }
    
    // samp /= counter;
    
    
    float baseHeight = tex0.w;
    vec3 worldPosition = tex0.xyz;
    vec4 worldPositionRef = vec4(0.0);
    
    
    float baseHeightWater = tex2.w;
    vec3 worldPositionWater = tex2.xyz;
    
    
    float camDis = clamp(distance(worldPositionWater,cameraPos)/16384.0,0.0,1.0);

    float distances[maxEntries];

    float difScale = mix(8.0,128.0,camDis);

    distances[0] = 0.0*difScale;
    distances[1] = 0.5*difScale;
    distances[2] = 2.0*difScale;
    distances[3] = 6.0*difScale;
    distances[4] = 8.0*difScale;
    distances[5] = 12.0*difScale;

    vec3 colVecs[maxEntries];

    colVecs[0] = vec3(0.7,0.9,1.0);
    colVecs[1] = vec3(0.0,1.0,1.0);
    colVecs[2] = vec3(0.0,0.4,0.8);
    colVecs[3] = vec3(0.0,0.2,0.5);
    colVecs[4] = vec3(0.0,0.1,0.3);
    colVecs[5] = vec3(0.0,0.0,0.1);
    
    vec3 waterNorm = -tex3.xyz;//-normalize((tex3.rgb-0.5)*2.0);
    
    float fEye = 1.0-clamp(dot(oneVec.xyz,-waterNorm.xyz),0.0,1.0);
    
    for (i = 0; i < maxEntries; i++) {
        colVecs[i].g += fEye*0.1;
    }

    float maxDis = distances[maxEntriesM1];

    float heightDif = 0.0;
    float heightDifNoRef = 0.0;

    

    //vec2 newTC2 = vec2(0.0);
    vec2 newTC = vec2(0.0);
    vec2 resTC = vec2(0.0);


    vec4 tex0Ref = vec4(0.0);
    vec4 tex1Ref = vec4(0.0);

    vec4 tex2Ref = vec4(0.0);
    vec4 tex3Ref = vec4(0.0);

    vec4 tex4Ref = vec4(0.0);
    vec4 tex5Ref = vec4(0.0);
    
    
    bool wasTrans = true;
    
    vec3 myRay = vec3(0.0);
    vec2 moveVec = vec2(0.0);
    vec2 moveVec2 = vec2(0.0);
    float newTime = 0.0;
        

    //vec4 tex5Ref2 = vec4(0.0);

    // vec4 tex7Ref = vec4(0.0);
    // vec4 tex7Ref2 = vec4(0.0);
    // vec4 tex7Ref3 = vec4(0.0);

    float baseHeightRef = 0.0;

    float myDis = (clamp(distance(worldPosition.xyz,cameraPos.xyz)*4.0/(clipDist.y),0.0,1.0));

    float curLerp = 0.0;
    float lval;

    float lerpVal;
    float lerpValNorm = 0.0;

    float totRef;

    float facingEye = 0.0;//clamp(dot(oneVec.xyz,-waterNorm.xyz),0.0,1.0);

    vec3 finalCol = vec3(0.0);

    vec3 lastCol = vec3(0.0);
    
    vec3 refMod = vec3(0.0);
    
    
    
    vec4 matValsSolid = vec4(0.0,0.0,pack16(tex1.w));
    vec4 matValsWater = vec4(0.0,0.0,pack16(tex3.w));
    
    
    vec3 hsvVal = rgb2hsv(tex10.rgb);
    float lightRes = max(max(tex10.r,tex10.g),tex10.b);
    
    vec3 transRendered = unpackColor(matValsWater.ba, lightRes);
    
    // transRendered =
    // mix(
    //     mix(
    //             transRendered*0.5,
    //             transRendered*(tex10.w*0.5+0.5),
    //             lightRes//tex2.rgb
    //     ),
    //     tex10.rgb,
    //     hsvVal.g*0.5
    // );
    




    heightDifNoRef = clamp((baseHeightWater - baseHeight)*clipDist.y, 0.0, maxDis);
    newTC.xy = TexCoord0.xy + mix(0.06,0.01,clamp(camDis*16.0,0.0,1.0) )*(waterNorm.xy-waterNorm.z)*heightDifNoRef/maxDis;
    
    tex0Ref = texture2D(Texture0, newTC.xy);
    
    //tex0Ref.w = max(tex0Ref.w,texture2D(Texture13, newTC.xy).w);
    
    tex1Ref = texture2D(Texture1, newTC.xy);
    tex5Ref = texture2D(Texture5, newTC.xy);
    
    worldPositionRef = tex0Ref;
    baseHeightRef = worldPositionRef.w;


    
    
    
   

    totRef = float(tex0Ref.w != 0.0);
    // if (totRef == 0.0) {
    //     baseHeightRef = baseHeightWater-1.0;
    // }

    heightDif = clamp(abs(baseHeightWater - baseHeightRef)*clipDist.y*0.5, 0.0, maxDis);

    lval = dot(oneVec.rgb, tex5Ref.rgb)/3.0;


    refMod = tex1Ref.xyz;//normalize((tex1Ref.xyz-0.5)*2.0)/2.0;
    refMod.xy += refMod.z;
    //refMod *= 0.5;

    // tex7Ref = texture2D(Texture7, newTC.xy);
    // tex7Ref2 = texture2D(Texture7, newTC.xy + refMod.xy*0.1 + tex7Ref.xy - baseHeight*clipDist.y*0.1/bufferDim.xy );
    // tex7Ref3 = texture2D(Texture7, newTC.xy + refMod.xy*0.1 + 1.0 - (tex7Ref.xy - baseHeight*clipDist.y*0.1/bufferDim.xy) );


    // bigger == deeper under water

    
    // !!!!

    // if (tex3.a == 0.0 ) {
    //     heightDif = max(heightDif,distances[1]+1.0);
    // }

    float curTOD = 1.0; //timeOfDay


    if (matValsWater.a == TEX_WATER) {
        
        if ( false ) {//isUnderWater == 1.0) {
            // this gets overwritten
            //finalCol = mix(tex4.rgb,tex5Ref.rgb,myDis);
        }
        else {
            

            for (i = 0; i < maxEntriesM1; i++) {

                if (
                    (heightDif >= distances[i]) &&
                    (heightDif <= distances[i+1]) 
                ) {

                    

                    lerpVal = clamp( (heightDif - distances[i])/(distances[i+1]-distances[i]), 0.0, 1.0);
                    lerpValNorm = heightDif/maxDis;
                    
                    lastCol = mix(colVecs[i], colVecs[i+1], lerpVal);
                    finalCol = lastCol*lval;
                    
                    

                    

                }
            }

            // if (tex3.a == 0.0 ) {

            //     resTC = TexCoord0.xy;

            //     if ( abs(waterNorm.z) > 0.5) {
                    
            //     }
            //     else {
                    
            //         // rays

            //         // finalCol += sin(
            //         //     (worldPositionWater.x + worldPositionWater.y + worldPositionWater.z/2.0)/(100.0) + curTime/500.0
            //         // )*vec3(0.2,0.2,0.4);
            //         // * pow(1.0-clamp((curSeaLev-worldPositionWater.z)/2000.0,0.0,1.0),5.0 )*mix(0.5,1.0,curTOD);

            //     }

            //     // caustics


            //     finalCol += pow(
            //     (
            //         //pow(abs(sin(tex7Ref.a*10.0)),4.0) +
            //         pow( ( (tex7Ref2.a ) ), 2.0 ) +
            //         pow( ( (tex7Ref3.a ) ), 2.0 ) 
            //     ) *
            //     clamp( 1.0 - ((baseHeightWater - baseHeightRef)/512.0),0.0,1.0 )*
            //     totRef
            //     ,2.0)*vec3(0.2,0.2,0.4)*mix(0.2,1.0,curTOD)*lval;
                
                


            // //     // bubbles

            //     finalCol += 
            //     pow(
            //         texture2D(Texture6, resTC*vec2(1.0,1.0)-vec2(0.0,(curTime/20000.0))  ).rgb*0.85,
            //         vec3(10.0)
            //     ) * 
            //     abs(sin(rand(TexCoord0.xy)*1000.0 + curTime/200.0))*(1.0-lerpValNorm)*mix(0.75,1.0,curTOD);



            // }
            // else {
                resTC = newTC.xy;
                



                // finalCol = mix(
                //     finalCol,
                //     finalCol*vec3(0.4,0.5,0.6) + transRendered.rgb,
                //     facingEye*0.3
                // ) + transRendered.rgb*vec3(0.1,0.2,0.4);


                // // highlights

                finalCol += 
                pow(
                    clamp(dot(
                        -waterNorm,
                        vec3(0.0,0.0,1.0)
                    ),0.0,1.0),
                    6.0
                )*vec3(0.5,0.45,1.0)*0.25*curTOD-0.125;

                
                finalCol *= 0.5;
                finalCol += tex5Ref.rgb*0.1;
                
                finalCol = mix(finalCol,transRendered.rgb,0.3*facingEye);
                
                finalCol = (finalCol + lastCol*mix(vec3(0.05,0.1,0.2),vec3(0.5),curTOD));

                if (
                    (heightDifNoRef >= distances[0]) &&
                    (heightDifNoRef <= distances[1]) &&
                    (isUnderWater == 0.0)
                ) {
                    // shore foam

                    //finalCol = vec3(1.0);
                    
                    lerpVal = clamp( (heightDifNoRef - distances[0])/(distances[1]-distances[0]), 0.0, 1.0);
                    finalCol += 1.0*finalCol*(rand(TexCoord0.xy*curTime/10000.0)*0.15+0.25)*(1.0-lerpVal)*4.0*mix(0.5,1.0,curTOD);
                
                }



            

            
            
            //finalCol *= 0.5;
            
            //finalCol = vec3(max(max(tex10.r,tex10.g),tex10.b));
            
            
        }
        
    }
    else {

        if (matValsWater.a == TEX_GLASS) {
            finalCol = transRendered.rgb*0.5 + tex5.rgb*0.25 + tex4.rgb*0.5;
            
        }
        else {
            
            if (tex2.w > tex0.w) {
                finalCol = transRendered.rgb*0.75 + tex5.rgb*0.25 + tex4.rgb*0.25;
            }
            else {
                wasTrans = false;
                finalCol = tex4.rgb;
            }
            
            
        }


        
    }
    
    
    
    
    
    //finalCol.rgb = unpackColor(matValsWater.ba, lightRes);
    
    if (wasTrans) {
        finalCol.rgb += finalCol.rgb*pow(tex11.g,8.0)*2.0;
        
        //finalCol = vec3(tex11.g);
    }
    
    if (isUnderWater == 1.0) {
        
        //finalCol = tex4.rgb;
        //if (matValsWater.a == TEX_WATER) {
        //    finalCol = tex5Ref.rgb;
        //}
            
        // if (matValsWater.a == TEX_WATER) {
        //     finalCol += vec3(0.0,0.1,0.3);//tex5.rgb;
        // }
        
        //bubbles
        // if (
        //     (matValsWater.a != TEX_WATER) ||
        //     (
        //         distance(worldPosition,worldPositionWater) < 5.0    
        //     )    
        // ) {
            
            if (matValsWater.a != TEX_WATER) {
                heightDif = clamp((seaLevel - worldPosition.z)*0.3,0.0,maxDis);
                
                for (i = 0; i < maxEntriesM1; i++) {

                    if (
                        (heightDif >= distances[i]) &&
                        (heightDif <= distances[i+1]) 
                    ) {

                        

                        lerpVal = clamp( (heightDif - distances[i])/(distances[i+1]-distances[i]), 0.0, 1.0);
                        lerpValNorm = heightDif/maxDis;
                        
                        lastCol = mix(colVecs[i], colVecs[i+1], lerpVal);
                        finalCol = lastCol*lval;       

                    }
                }
            }
            else {
                finalCol += tex5Ref.rgb*0.25;
            }
            
            
            
            
            

        //}
    }
    
    
    
    vec4 charTest;
    
    // vec4 objSamp = texture2D(Texture13, TexCoord0.xy);
    
    // if (matValsWater.a == TEX_WATER) {
        
        
    //     if (
    //         (objSamp.w < baseHeightWater)
    //     ) {
    //         charTest = texture2D(Texture14, mix(newTC.xy,TexCoord0,0.75) );
    //         if (charTest.a == 0.0) {
                
    //         }
    //         else {
    //             finalCol = mix(charTest.rgb,finalCol,0.9);
    //         }
            
    //     }
        
    //     finalCol.rgb = mix(pow(finalCol.rgb,vec3(2.0)), finalCol.rgb, timeOfDay);
        
    // }
    // else {
    //     if (objSamp.w > baseHeight) {
    //         finalCol = texture2D(Texture14, TexCoord0.xy).rgb;
            
    //         finalCol = mix(finalCol, transRendered.rgb*0.75 + tex5.rgb*0.25 + tex4.rgb*0.25, 0.25);
            
    //     }
    // }
    
    
    
    
    
    
    gl_FragData[0] = vec4(finalCol.rgb,1.0);
    
}
