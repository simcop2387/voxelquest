uniform sampler2D Texture0;
uniform sampler2D Texture1;

uniform vec2 bufferDim;
uniform float heightOfNearPlane;


uniform float boxDiam;
uniform mat4 modelview;
uniform float focalLength;
uniform vec3 cameraPos;

// struct Ray {
//     vec3 Origin;
//     vec3 Dir;
// };

// struct AABB {
//     vec3 Min;
//     vec3 Max;
// };

////////




varying vec2 TexCoord0;

$

void main() {
    TexCoord0 = gl_MultiTexCoord0.xy;
    gl_Position = gl_Vertex;
}

$






// //via http://prideout.net/blog/?p=64

// vec3 IntersectBox(Ray r, AABB aabb) {
    
//     float t0;
//     float t1;
    
//     vec3 invR = 1.0 / r.Dir;
//     vec3 tbot = invR * (aabb.Min-r.Origin);
//     vec3 ttop = invR * (aabb.Max-r.Origin);
//     vec3 tmin = min(ttop, tbot);
//     vec3 tmax = max(ttop, tbot);
//     vec2 t = max(tmin.xx, tmin.yz);
//     t0 = max(t.x, t.y);
//     t = min(tmax.xx, tmax.yz);
//     t1 = min(t.x, t.y);
//     return vec3(t0,t1,float(t0 <= t1));
// }



const int rad = 8;
const float frad = float(rad);

void main() {

    vec4 tex0 = texture2D(Texture0, TexCoord0 );
    vec4 tex1 = texture2D(Texture1, TexCoord0 );
    vec4 tex2 = vec4(0.0);
    vec4 tex3 = vec4(0.0);
    
    vec4 oneVec = vec4(1.0);
    
    vec4 texf0 = vec4(0.0);
    vec4 texf1 = vec4(0.0);

    int i;
    int j;
    
    float curRad = 0.0;
    
    float bestW = 0.0;
    float bestW2 = 0.0;
    float bestW3 = 0.0;
    vec4 samp = vec4(0.0);
    vec2 tc = vec2(0.0);
    vec2 bestCoord = TexCoord0;
    vec2 bestCoord2 = TexCoord0;
    vec2 bestCoord3 = TexCoord0;
    vec2 curCoord = vec2(0.0);
    vec2 offVal = vec2(0.0);
    vec2 orig = vec2(0.0);
    
    float testW = 0.0;
    float pSize = 0.0;
    
    float airCount = 0.0;
    float totCount = 0.0;
    
    
    vec3 worldPos = vec3(0.0);
    
    // clamp((1.0-samp.w)/0.2,0.0,1.0)
    
    
    
    
    
    for (j = -rad; j <= rad; j++ ) {
        tc.y = float(j);
        for (i = -rad; i <= rad; i++ ) {
            tc.x = float(i);
            
            offVal = tc/bufferDim;
            curCoord = TexCoord0 + offVal;
            samp = texture2D(Texture0,curCoord);
            
            curRad = length(tc.xy/frad);
            
            if (curRad < 1.0) {
                if (samp.w > 0.0) {
                    testW = samp.w;
                    if (testW > bestW) {                
                        bestW = testW;
                    }
                }
            }
            
            
            
            
            
        }  
    }
    
    float closestW = bestW;
    bestW = 0.0;
    testW = 0.0;
    
    
    
    
    // int newRad = 
    // int(mix(
    //     8.0,
    //     0.0,
    //     clamp(
    //         (1.0-closestW)/0.6,
    //         0.0,
    //         1.0
    //     )
    // ));
    // //rad;
    
    // float fNewRad = float(newRad);
    
    vec4 avgVal0 = vec4(0.0);
    vec4 avgVal1 = vec4(0.0);
    float totVal = 0.0;
    float tempVal = 0.0;
    
    for (j = -rad; j <= rad; j++ ) {
        tc.y = float(j);
        for (i = -rad; i <= rad; i++ ) {
            tc.x = float(i);
            
            offVal = tc/bufferDim;
            curCoord = TexCoord0 + offVal;
            
            samp = texture2D(Texture0,curCoord);
            
            //curRad = length(tc.xy/frad);
            // if (curRad < 1.0) {
            //     if (samp.w >= closestW-0.001) {
                    
            //         testW = (1.0-curRad);//samp.w*mix(1.0,0.9999,curRad);

            //         if (testW > bestW) {
                        
            //             bestW2 = bestW;
            //             bestCoord2 = bestCoord;
                        
            //             bestW = testW;
            //             bestCoord = curCoord;
            //         }
            //         else {
            //             if (testW > bestW2) {
            //                 bestW2 = testW;
            //                 bestCoord2 = curCoord;
            //             }
            //         }
                    
            //     }
            // }
            
            if (samp.w > (closestW-0.2)) {
                tempVal = clamp(1.0-length(tc.xy/frad),0.0,1.0);
                totVal += tempVal;
                avgVal0 += samp*tempVal;
                avgVal1 += texture2D(Texture1,curCoord)*tempVal;
            }
            
            
            
            //airCount += float(samp.w == 0.0);
            //totCount += 1.0;
            
            
        }  
    }
    
    
    // if (bestW2 == 0.0) {
    //     bestCoord2 = bestCoord;
    // }
    
    // float d1 = distance(TexCoord0,bestCoord);
    // float d2 = distance(TexCoord0,bestCoord2);
    
    // float lerpVal = clamp(d1/(d1+d2),0.0,1.0);
    
    // tex0 = texture2D(Texture0,bestCoord);
    // tex1 = texture2D(Texture1,bestCoord);
    
    // tex2 = texture2D(Texture0,bestCoord2);
    // tex3 = texture2D(Texture1,bestCoord2);
    
    
    // texf0 = mix(tex0,tex2,lerpVal);
    // texf1 = mix(tex1,tex3,lerpVal);
    
    if (totVal == 0.0) {
        texf0 = tex0;
        texf1 = tex1;
    }
    else {
        texf0 = avgVal0/totVal;
        texf1 = avgVal1/totVal;
        texf1.xyz = normalize(texf1.xyz);
        texf1.w = tex1.w;
        
        //texf0 = mix(texf0,tex0,0.5);
        //texf1 = mix(texf1,tex1,0.5);
    }
    
   
   
    
    // if (dot(abs(texf1.xyz),oneVec.xyz) != 0.0) {
    //     texf1.xyz = normalize(texf1.xyz);
    // }
    
    
    
    
    
    gl_FragData[0] = texf0;
    gl_FragData[1] = texf1;


}
