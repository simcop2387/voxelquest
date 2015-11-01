#version 330


uniform float volSizePrim;
uniform float FOV;
uniform vec2 clipDist;
uniform vec2 bufferDim;
uniform vec3 cameraPos;
uniform vec3 volMinReadyInPixels;
uniform vec4 maskVals;
uniform vec3 lookAtVec;

uniform mat4 modelviewInverse;
uniform mat4 proj;
uniform mat4 modelview;


//uniform sampler2D Texture0;

// uniform sampler3D Texture0;
// uniform sampler2D Texture2;
// uniform sampler3D Texture13;

$


layout(location = 0) in vec4 vposition;
layout(location = 1) in vec4 vtexcoord;
//out vec4 TexCoord0;
out vec4 worldPos;


void main() {
    
    worldPos = vtexcoord;
    gl_Position = proj*modelview*vec4(vposition.xyz,1.0); ///clipDist.y
    
    //gl_Position.z /= gl_Position.w; //
    //gl_Position.z = smoothstep(clipDist.y,clipDist.y,gl_Position.z);
    
}


$

in vec4 worldPos;
//in vec4 TexCoord0;
layout(location = 0) out vec4 FragColor0;


// // vec3 getRay() {
// // 	vec2 baseCoords = gl_FragCoord.xy/(bufferDim.xy);
	
// //     float aspect = bufferDim.y/bufferDim.x;
// //     float NEAR = clipDist.x;
// //     float FAR = clipDist.y;
// //     float dx = tan(FOV*0.5f)*(baseCoords.x*2.0-1.0f)/aspect; //gl_FragCoord.x/(bufferDim.x*0.5)
// //     float dy = tan(FOV*0.5f)*((1.0f-baseCoords.y)*2.0-1.0f); //gl_FragCoord.y/(bufferDim.y*0.5)
    
    
// //     dx = -dx;
    
// //     vec4 p1 = vec4(dx*NEAR,dy*NEAR,NEAR,1.0);
// //     vec4 p2 = vec4(dx*FAR,dy*FAR,FAR,1.0);
    
// //     p1 = modelviewInverse*p1;
// //     p2 = modelviewInverse*p2;

// //     vec3 ro = p1.xyz;
    
// //     vec3 rd = normalize(p1.xyz-p2.xyz);
    
// //     return rd;
// // }



// //! Tricubic interpolated texture lookup, using unnormalized coordinates.
// //! Fast implementation, using 8 trilinear lookups.
// //uniform vec3 nrOfVoxels;

// // todo: add license:
// // https://github.com/DannyRuijters/CubicInterpolationCUDA/blob/master/examples/glCubicRayCast/tricubic.shader

// vec4 tricubic(sampler3D mySampler, vec3 coord, vec3 nrOfVoxels)
// {
//     // shift the coordinate from [0,1] to [-0.5, nrOfVoxels-0.5]
//     //vec3 nrOfVoxels = vec3(textureSize3D(tex, 0));
//     vec3 coord_grid = coord * nrOfVoxels - 0.5;
//     vec3 index = floor(coord_grid);
//     vec3 fraction = coord_grid - index;
//     vec3 one_frac = 1.0 - fraction;

//     vec3 w0 = 1.0/6.0 * one_frac*one_frac*one_frac;
//     vec3 w1 = 2.0/3.0 - 0.5 * fraction*fraction*(2.0-fraction);
//     vec3 w2 = 2.0/3.0 - 0.5 * one_frac*one_frac*(2.0-one_frac);
//     vec3 w3 = 1.0/6.0 * fraction*fraction*fraction;

//     vec3 g0 = w0 + w1;
//     vec3 g1 = w2 + w3;
//     vec3 mult = 1.0 / nrOfVoxels;
//     vec3 h0 = mult * ((w1 / g0) - 0.5 + index);  //h0 = w1/g0 - 1, move from [-0.5, nrOfVoxels-0.5] to [0,1]
//     vec3 h1 = mult * ((w3 / g1) + 1.5 + index);  //h1 = w3/g1 + 1, move from [-0.5, nrOfVoxels-0.5] to [0,1]

//     // fetch the eight linear interpolations
//     // weighting and fetching is interleaved for performance and stability reasons
//     vec4 tex000 = texture(mySampler, h0);
//     vec4 tex100 = texture(mySampler, vec3(h1.x, h0.y, h0.z));
//     tex000 = mix(tex100, tex000, g0.x);  //weigh along the x-direction
//     vec4 tex010 = texture(mySampler, vec3(h0.x, h1.y, h0.z));
//     vec4 tex110 = texture(mySampler, vec3(h1.x, h1.y, h0.z));
//     tex010 = mix(tex110, tex010, g0.x);  //weigh along the x-direction
//     tex000 = mix(tex010, tex000, g0.y);  //weigh along the y-direction
//     vec4 tex001 = texture(mySampler, vec3(h0.x, h0.y, h1.z));
//     vec4 tex101 = texture(mySampler, vec3(h1.x, h0.y, h1.z));
//     tex001 = mix(tex101, tex001, g0.x);  //weigh along the x-direction
//     vec4 tex011 = texture(mySampler, vec3(h0.x, h1.y, h1.z));
//     vec4 tex111 = texture(mySampler, h1);
//     tex011 = mix(tex111, tex011, g0.x);  //weigh along the x-direction
//     tex001 = mix(tex011, tex001, g0.y);  //weigh along the y-direction

//     return mix(tex001, tex000, g0.z);  //weigh along the z-direction
// };

// vec4 getTexLin(vec3 pos) {
    
//     return texture(Texture0, (pos)/volSizePrim );
// }

// vec4 getTexCubic(sampler3D mySampler, vec3 pos, float vs) {
    
//     return tricubic(mySampler,(pos)/vs, vec3(vs) );
    
// }

// float opD( float d1, float d2 )
// {
//     return d1+d2;
// }
// vec2 opU( vec2 d1, vec2 d2 )
// {
//     return mix(d2,d1,float(d1.x<d2.x));
// }

// vec2 getTerrain(
//     vec2 pRes,
//     vec3 terNorm,
//     vec3 pos
// ) {
    
//     int i;
//     float fi;
    
//     vec2 res = pRes;
    
//     float primRes = res.x;
    
    
//     vec3 absTerNorm = abs(terNorm);
    
    
//     vec3 cellSize = vec3(0.5);
//     float jutDis = 0.0;
//     vec3 gradVal;
    
    
//     float texVal;
//     float lerpVal;
    
//     vec3 norVal = vec3(0.0);
    
//     vec3 repPos = vec3(0.0);
//     float oldX = res.x;
//     float cellX;
    
    
//     float randVal = mod(
//                 abs(
//                     sin(pos.z/32.0)*
//                     sin(pos.x/32.0)*
//                     sin(pos.y/32.0)
//                 ),
//                 1.0
//             );
    
//     float upTex;
    
//     vec3 posSamp = pos/2.0;
    
//     float seedRand = (sin((posSamp.x*6.2 - posSamp.y*2.6)/20.0)+1.0) + 
//                     (sin((posSamp.y*3.4 + posSamp.z*1.1)/30.0)+1.0) +
//                     (sin((posSamp.z*4.4 + posSamp.x*2.1)/30.0)+1.0);
//     seedRand = mix(seedRand/6.0,randVal,0.5);
    
//     seedRand = (sin(seedRand*8.0)+1.0)/2.0;
    
//     float camDetail = clamp(distance(pos,cameraPos)/clipDist.y,0.0,1.0);
    
//     float texSpacing = 100.0*@myTexSpacing@;




//     oldX = res.x;

//     upTex = texture(Texture2, pos.xy/texSpacing).r;
//     texVal = 
//         texture(Texture2, pos.yz/texSpacing).r*absTerNorm.x*absTerNorm.x +
//         texture(Texture2, pos.zx/texSpacing).r*absTerNorm.y*absTerNorm.y +
//         upTex*absTerNorm.z*absTerNorm.z;

//     texVal *= 3.0*@myTexTest@;

//     float scaleFactor = 2.0;

    
//     #ifdef DOVORO
//     jutDis = clamp(
        
//         ((
//                          distance(pos.z, cellVal.z-cellSize.z ) / 
//                          (cellSize.z)
//         )) * clamp(mix(0.0,1.5,gradVal.x),0.0,1.0),
//         0.0,
//         1.0
//     );

//     jutDis = 1.0-jutDis;
//     #endif



//     oldX = res.x;

//     float myVal = clamp(
//         pow( (1.0-getTexCubic(Texture13, pos*4.0 + randVal*16.0, voroSize).r), 4.0 )
//         *0.4*pow(1.0-camDetail,4.0),0.0,1.0);

//     res.x = opD(
//         res.x,
//         myVal
//     );

    

//     res.x = opD(
//         res.x,
//         pow(texVal,2.0) * mix(0.4,0.5,camDetail)
//     );
    


//     res.x = mix(res.x,opD(pRes.x,texVal*0.5),pow(seedRand,3.0));

//     res.x = mix(
//         res.x,
//         oldX + texVal*(0.15+seedRand*0.35),
//         pow(seedRand,2.0)
//     );
    
//     return res;
// }


// float getLand3D(vec3 pos, float sampVal) {
    
    
//     float maxDis = mix(0.5,1.0,distance(cameraPos,pos)/clipDist.y);
    
//     return mix(
//         maxDis,
//         -maxDis,
//         (sampVal) //samp.g //+sin(curTime)*0.1
//     );
// }


// float mapLand(vec3 pos) {
    
//     int i;
    
//     vec2 ov = vec2(0.0,0.5);
//     vec2 res = vec2(clipDist.y,0.0);
//     float baseRes = getTexCubic(Texture0,pos-volMinReadyInPixels, volSizePrim).r;
//     float landDis = getLand3D(pos, baseRes);
//     vec3 terNorm = normalize(
//         vec3(baseRes) - 
//         vec3(
//             getTexLin( (pos+ov.yxx)-volMinReadyInPixels ).r,
//             getTexLin( (pos+ov.xyx)-volMinReadyInPixels ).r,
//             getTexLin( (pos+ov.xxy)-volMinReadyInPixels ).r
//         )
//     );
    
//     res = opU(
//         res,
//         vec2(
//             landDis,
//             0.0
//         )
//     );
    
//     if (res.x <= 0.1) {
//         res = getTerrain(res, terNorm, pos);
//     }
    
//     return res.x;
// }


void main() {
    
    // vec2 baseCoords = gl_FragCoord.xy/(bufferDim.xy);
    
    // vec4 res = texture(Texture0,baseCoords);
    
    // vec4 maskInverse = vec4(1.0)-maskVals;
    // res = res*maskInverse;
    //res = vec4(1.0);
    
	FragColor0 = maskVals*distance(worldPos.xyz,cameraPos.xyz);
    
}
