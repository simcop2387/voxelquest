
void getRay(in vec2 newTC, inout vec3 ro, inout vec3 rd) {
 float aspect = bufferDim.y/bufferDim.x;
 float NEAR = clipDist.x;
 float FAR = clipDist.y;

 float dx = tan(FOV*0.5f)*(newTC.x*2.0-1.0f)/aspect;
 float dy = tan(FOV*0.5f)*((1.0f-newTC.y)*2.0-1.0f);

 dx = -dx;

 vec4 p1 = vec4(dx*NEAR,dy*NEAR,NEAR,1.0);
 vec4 p2 = vec4(dx*FAR,dy*FAR,FAR,1.0);

 p1 = modelviewInverse*p1;
 p2 = modelviewInverse*p2;

 ro = p1.xyz;
 rd = normalize(p1.xyz-p2.xyz);
}

vec2 aabbIntersect(vec3 rayOrig, vec3 rayDir, vec3 minv, vec3 maxv) {
    float t0;
    float t1;

    vec3 invR = 1.0 / rayDir;
    vec3 tbot = invR * (minv-rayOrig);
    vec3 ttop = invR * (maxv-rayOrig);
    vec3 tmin = min(ttop, tbot);
    vec3 tmax = max(ttop, tbot);
    vec2 t = max(tmin.xx, tmin.yz);
    t0 = max(t.x, t.y);
    t = min(tmax.xx, tmax.yz);
    t1 = min(t.x, t.y);
    return vec2(t0,t1); // if (t0 <= t1) { did hit } else { did not hit }
}
