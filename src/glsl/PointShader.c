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





void main() {

    vec4 tex0 = texture2D(Texture0, TexCoord0 );
    vec4 tex1 = texture2D(Texture1, TexCoord0 );


    int i;
    int j;
    int rad = 8;
    int radStep = 2;
    float frad = float(rad);
    float curRad = 0.0;
    
    float bestW = 999999.0;
    vec4 samp = vec4(0.0);
    vec2 tc = vec2(0.0);
    vec2 bestCoord = TexCoord0;
    vec2 curCoord = vec2(0.0);
    vec2 orig = vec2(0.0);
    
    float testW = 0.0;
    float pSize = 0.0;
    
    float airCount = 0.0;
    float totCount = 0.0;
    
    
    float boxRad = boxDiam/2.0;
    vec3 worldPos = vec3(0.0);
    
    
    // /////////////////////
    
    
    // vec3 rayDirection = vec3(0.0);
    // vec3 res = vec3(0.0);
    // vec3 rayStart = vec3(0.0);
    // float tnear = 0.0;
    // float tfar = 0.0;
    // bool didHit = false;
    
    // rayDirection.xy = 2.0 * gl_FragCoord.xy / bufferDim - 1.0;
    // rayDirection.z = -focalLength;
    // rayDirection = (vec4(rayDirection, 0.0) * modelview).xyz;

    // Ray eye = Ray( cameraPos, normalize(rayDirection) );
    // AABB aabb;// = AABB(worldPos-boxRad, worldPos+boxRad);

    // // aabb.Min = worldPos-boxRad;
    // // aabb.Max = worldPos+boxRad;
    // // res = IntersectBox(eye, aabb);
    // // tnear = max(res.x,0.0);
    // // tfar = res.y;
    // // didHit = bool(res.z);
    // // rayStart = eye.Origin + eye.Dir * tnear;
    
    
    // // vec3 rayStop = eye.Origin + eye.Dir * tfar;
    
    // // // Transform from object space to texture coordinate space:
    // // rayStart = 0.5 * (rayStart + 1.0);
    // // rayStop = 0.5 * (rayStop + 1.0);
    
    
    
    
    
    // /////////////////////
    
    
    
    
    
    
    
    
    if (tex0.w == 0.0) {
        for (j = -rad; j <= rad; j+=radStep ) {
            tc.y = float(j);
            for (i = -rad; i <= rad; i+=radStep ) {
                tc.x = float(i);
                
                curCoord = TexCoord0 + tc/bufferDim;
                
                samp = texture2D(Texture0,curCoord);
                
                worldPos = samp.xyz;
                //pSize = (samp.w)*2.0;//(heightOfNearPlane) / (samp.w); //heightOfNearPlane
                curRad = length(tc.xy);
                
                
                
                if (samp.w != 0.0) {
                    // aabb.Min = worldPos-boxRad;
                    // aabb.Max = worldPos+boxRad;
                    // res = IntersectBox(eye, aabb);
                    // tnear = max(res.x,0.0);
                    // tfar = res.y;
                    // didHit = bool(res.z);
                    // rayStart = eye.Origin + eye.Dir * tnear;
                    // vec3 rayStop = eye.Origin + eye.Dir * tfar;
                    
                    // // // Transform from object space to texture coordinate space:
                    // rayStart = 0.5 * (rayStart + 1.0);
                    // rayStop = 0.5 * (rayStop + 1.0);
                    
                    testW = distance(cameraPos,worldPos);
                    
                    //if (res.z != 0.0) {
                        //testW = distance(rayStart,TexCoord0); //tnear;//distance(rayStart,cameraPos);
                        
                        if ((testW < bestW)) { //&&(curRad < samp.w)
                        //if (didHit) {
                            bestW = testW;
                            bestCoord = curCoord;
                        }
                    //}
                    
                    
                    
                }
                
                
                
                
                
                
                //pSize = (heightOfNearPlane*0.01) * samp.w;
                
                // w approaches 1.0 as it gets closer to camera
                
                //testW = samp.w - distance(tc,orig)*0.1/frad;// + (1.0-distance(tc,orig)/frad )*0.01;///abs(samp.w-tex0.w);// + (1.0-distance(tc,orig)/frad )*samp.w;
                
                //airCount += float(samp.w == 0.0);
                //totCount += 1.0;
                
                
            }  
        }
    }
    
    
    
    
    

    tex0 = texture2D(Texture0,bestCoord);
    tex1 = texture2D(Texture1,bestCoord);
    
    
    
    
    
    gl_FragData[0] = tex0;
    gl_FragData[1] = tex1;


}
