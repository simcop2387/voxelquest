// Tricubic interpolated texture lookup, using unnormalized coordinates.
// Fast implementation, using 8 trilinear lookups.
// uniform vec3 nrOfVoxels;

// todo: add license:
// https://github.com/DannyRuijters/CubicInterpolationCUDA/blob/master/examples/glCubicRayCast/tricubic.shader

vec4 tricubic(sampler3D mySampler, vec3 coord, vec3 nrOfVoxels)
{
		// shift the coordinate from [0,1] to [-0.5, nrOfVoxels-0.5]
		//vec3 nrOfVoxels = vec3(textureSize3D(tex, 0));
		vec3 coord_grid = coord * nrOfVoxels - 0.5;
		vec3 index = floor(coord_grid);
		vec3 fraction = coord_grid - index;
		vec3 one_frac = 1.0 - fraction;

		vec3 w0 = 1.0/6.0 * one_frac*one_frac*one_frac;
		vec3 w1 = 2.0/3.0 - 0.5 * fraction*fraction*(2.0-fraction);
		vec3 w2 = 2.0/3.0 - 0.5 * one_frac*one_frac*(2.0-one_frac);
		vec3 w3 = 1.0/6.0 * fraction*fraction*fraction;

		vec3 g0 = w0 + w1;
		vec3 g1 = w2 + w3;
		vec3 mult = 1.0 / nrOfVoxels;
		vec3 h0 = mult * ((w1 / g0) - 0.5 + index);  //h0 = w1/g0 - 1, move from [-0.5, nrOfVoxels-0.5] to [0,1]
		vec3 h1 = mult * ((w3 / g1) + 1.5 + index);  //h1 = w3/g1 + 1, move from [-0.5, nrOfVoxels-0.5] to [0,1]

		// fetch the eight linear interpolations
		// weighting and fetching is interleaved for performance and stability reasons
		vec4 tex000 = texture(mySampler, h0);
		vec4 tex100 = texture(mySampler, vec3(h1.x, h0.y, h0.z));
		tex000 = mix(tex100, tex000, g0.x);  //weigh along the x-direction
		vec4 tex010 = texture(mySampler, vec3(h0.x, h1.y, h0.z));
		vec4 tex110 = texture(mySampler, vec3(h1.x, h1.y, h0.z));
		tex010 = mix(tex110, tex010, g0.x);  //weigh along the x-direction
		tex000 = mix(tex010, tex000, g0.y);  //weigh along the y-direction
		vec4 tex001 = texture(mySampler, vec3(h0.x, h0.y, h1.z));
		vec4 tex101 = texture(mySampler, vec3(h1.x, h0.y, h1.z));
		tex001 = mix(tex101, tex001, g0.x);  //weigh along the x-direction
		vec4 tex011 = texture(mySampler, vec3(h0.x, h1.y, h1.z));
		vec4 tex111 = texture(mySampler, h1);
		tex011 = mix(tex111, tex011, g0.x);  //weigh along the x-direction
		tex001 = mix(tex011, tex001, g0.y);  //weigh along the y-direction

		return mix(tex001, tex000, g0.z);  //weigh along the z-direction
};

vec4 cubic(float v)
{
		vec4 n = vec4(1.0, 2.0, 3.0, 4.0) - v;
		vec4 s = n * n * n;
		vec4 res;
		res.x = s.x;
		res.y = s.y - 4.0 * s.x;
		res.z = s.z - 4.0 * s.y + 6.0 * s.x;
		res.w = 6.0 - res.x - res.y - res.z;
		return res;
}

// vec4 bilin(sampler2D mySampler, vec2 texcoord, vec2 texscale) {
// 	return texture(mySampler,texcoord);
// }


// from CodeProject.com
// float CatMullRom( float x )
// {
//     const float B = 0.0;
//     const float C = 0.5;
//     float f = x;
//     if( f < 0.0 )
//     {
//         f = -f;
//     }
//     if( f < 1.0 )
//     {
//         return ( ( 12 - 9 * B - 6 * C ) * ( f * f * f ) +
//             ( -18 + 12 * B + 6 *C ) * ( f * f ) +
//             ( 6 - 2 * B ) ) / 6.0;
//     }
//     else if( f >= 1.0 && f < 2.0 )
//     {
//         return ( ( -B - 6 * C ) * ( f * f * f )
//             + ( 6 * B + 30 * C ) * ( f *f ) +
//             ( - ( 12 * B ) - 48 * C  ) * f +
//             8 * B + 24 * C)/ 6.0;
//     }
//     else
//     {
//         return 0.0;
//     }
// } 

vec4 bilin(in sampler2D t, in vec2 uvIn, in vec2 textureSize, in vec2 texelSize)
{
    vec2 f = fract( uvIn * textureSize );
    vec2 uv = floor(uvIn * textureSize)/textureSize;
    // f.x = CatMullRom(1.0-f.x);
    // f.y = CatMullRom(1.0-f.y);
    
    vec3 newTX = vec3(texelSize.xy,0.0);
    
    //uv += texelSize*0.06;    // <--- precision hack (anything higher breaks it)
    vec4 tl = texture2D(t, uv);
    vec4 tr = texture2D(t, uv + newTX.xz);
    vec4 bl = texture2D(t, uv + newTX.zy);
    vec4 br = texture2D(t, uv + newTX.xy);
    vec4 tA = mix( tl, tr, f.x );
    vec4 tB = mix( bl, br, f.x );
    return mix( tA, tB, f.y );
}

// vec4 nearest(in sampler2D t, in vec2 uvIn, in vec2 textureSize, in vec2 texelSize)
// {
//     //vec2 f = fract( uvIn * textureSize );
//     vec2 uv = floor(uvIn * textureSize)/textureSize;
    
//     return texture(t, uv);
    
// }

vec4 trilin(in sampler3D t, in vec3 uvIn, in vec3 textureSize, in vec3 texelSize)
{
    vec3 f = fract( uvIn * textureSize );
    vec3 uv = floor(uvIn * textureSize)/textureSize;
    
    // f.x = CatMullRom(1.0-f.x);
    // f.y = CatMullRom(1.0-f.y);
    
    vec4 newTX = vec4(texelSize.xyz,0.0);
    
    //uv += texelSize*0.06;    // <--- precision hack (anything higher breaks it)
    vec4 tl = texture(t, uv + newTX.www);
    vec4 tr = texture(t, uv + newTX.xww);
    vec4 bl = texture(t, uv + newTX.wyw);
    vec4 br = texture(t, uv + newTX.xyw);
    vec4 tA = mix( tl, tr, f.x );
    vec4 tB = mix( bl, br, f.x );
    vec4 tC = mix( tA, tB, f.y );
    
    tl = texture(t, uv + newTX.wwz);
    tr = texture(t, uv + newTX.xwz);
    bl = texture(t, uv + newTX.wyz);
    br = texture(t, uv + newTX.xyz);
    tA = mix( tl, tr, f.x );
    tB = mix( bl, br, f.x );
    vec4 tC2 = mix( tA, tB, f.y );
    
    return mix(tC,tC2,f.z);
    
}

vec4 bicubicNearest(sampler2D mySampler, vec2 texcoord, vec2 texscale, vec2 tsInv) {
		float fx = fract(texcoord.x);
		float fy = fract(texcoord.y);
		texcoord.x -= fx;
		texcoord.y -= fy;

		vec4 xcubic = cubic(fx);
		vec4 ycubic = cubic(fy);

		vec4 c = vec4(texcoord.x - 0.5, texcoord.x + 1.5, texcoord.y - 0.5, texcoord.y + 1.5);
		vec4 s = vec4(xcubic.x + xcubic.y, xcubic.z + xcubic.w, ycubic.x + ycubic.y, ycubic.z + ycubic.w);
		vec4 offset = c + vec4(xcubic.y, xcubic.w, ycubic.y, ycubic.w) / s;

		vec4 sample0 = bilin(mySampler, vec2(offset.x, offset.z)*2.0/texscale, texscale, tsInv);
		vec4 sample1 = bilin(mySampler, vec2(offset.y, offset.z)*2.0/texscale, texscale, tsInv);
		vec4 sample2 = bilin(mySampler, vec2(offset.x, offset.w)*2.0/texscale, texscale, tsInv);
		vec4 sample3 = bilin(mySampler, vec2(offset.y, offset.w)*2.0/texscale, texscale, tsInv);
		
		

		float sx = s.x / (s.x + s.y);
		float sy = s.z / (s.z + s.w);

		return mix(
				mix(sample3, sample2, sx),
				mix(sample1, sample0, sx), sy
		);
}

vec4 bicubic(sampler2D mySampler, vec2 texcoord, vec2 texscale, vec2 tsInv) {
		float fx = fract(texcoord.x);
		float fy = fract(texcoord.y);
		texcoord.x -= fx;
		texcoord.y -= fy;

		vec4 xcubic = cubic(fx);
		vec4 ycubic = cubic(fy);

		vec4 c = vec4(texcoord.x - 0.5, texcoord.x + 1.5, texcoord.y - 0.5, texcoord.y + 1.5);
		vec4 s = vec4(xcubic.x + xcubic.y, xcubic.z + xcubic.w, ycubic.x + ycubic.y, ycubic.z + ycubic.w);
		vec4 offset = c + vec4(xcubic.y, xcubic.w, ycubic.y, ycubic.w) / s;

		// vec4 sample0 = bilin(mySampler, vec2(offset.x, offset.z)*2.0/texscale, texscale, tsInv);
		// vec4 sample1 = bilin(mySampler, vec2(offset.y, offset.z)*2.0/texscale, texscale, tsInv);
		// vec4 sample2 = bilin(mySampler, vec2(offset.x, offset.w)*2.0/texscale, texscale, tsInv);
		// vec4 sample3 = bilin(mySampler, vec2(offset.y, offset.w)*2.0/texscale, texscale, tsInv);
		
		
		vec4 sample0 = texture(mySampler, vec2(offset.x, offset.z) * texscale);
		vec4 sample1 = texture(mySampler, vec2(offset.y, offset.z) * texscale);
		vec4 sample2 = texture(mySampler, vec2(offset.x, offset.w) * texscale);
		vec4 sample3 = texture(mySampler, vec2(offset.y, offset.w) * texscale);
		

		float sx = s.x / (s.x + s.y);
		float sy = s.z / (s.z + s.w);

		return mix(
				mix(sample3, sample2, sx),
				mix(sample1, sample0, sx), sy
		);
}

vec4 getTexCubic(sampler3D mySampler, vec3 pos, float vs) {
	return tricubic(mySampler,(pos)/vs, vec3(vs) );
}

vec4 getTexLin(sampler3D mySampler, vec3 pos, float vs) {
	return texture(mySampler, (pos)/vs );
}





// vec4 tricubicNearest(sampler3D mySampler, vec3 coord, vec3 nrOfVoxels)
// {
// 		// shift the coordinate from [0,1] to [-0.5, nrOfVoxels-0.5]
// 		//vec3 nrOfVoxels = vec3(textureSize3D(tex, 0));
// 		vec3 coord_grid = coord * nrOfVoxels - 0.5;
// 		vec3 index = floor(coord_grid);
// 		vec3 fraction = coord_grid - index;
// 		vec3 one_frac = 1.0 - fraction;

// 		vec3 w0 = 1.0/6.0 * one_frac*one_frac*one_frac;
// 		vec3 w1 = 2.0/3.0 - 0.5 * fraction*fraction*(2.0-fraction);
// 		vec3 w2 = 2.0/3.0 - 0.5 * one_frac*one_frac*(2.0-one_frac);
// 		vec3 w3 = 1.0/6.0 * fraction*fraction*fraction;

// 		vec3 g0 = w0 + w1;
// 		vec3 g1 = w2 + w3;
// 		vec3 mult = 1.0 / nrOfVoxels;
// 		vec3 h0 = mult * ((w1 / g0) - 0.5 + index);  //h0 = w1/g0 - 1, move from [-0.5, nrOfVoxels-0.5] to [0,1]
// 		vec3 h1 = mult * ((w3 / g1) + 1.5 + index);  //h1 = w3/g1 + 1, move from [-0.5, nrOfVoxels-0.5] to [0,1]

// 		// fetch the eight linear interpolations
// 		// weighting and fetching is interleaved for performance and stability reasons
// 		vec4 tex000 = texture(mySampler, h0);
// 		vec4 tex100 = texture(mySampler, vec3(h1.x, h0.y, h0.z));
// 		tex000 = mix(tex100, tex000, g0.x);  //weigh along the x-direction
// 		vec4 tex010 = texture(mySampler, vec3(h0.x, h1.y, h0.z));
// 		vec4 tex110 = texture(mySampler, vec3(h1.x, h1.y, h0.z));
// 		tex010 = mix(tex110, tex010, g0.x);  //weigh along the x-direction
// 		tex000 = mix(tex010, tex000, g0.y);  //weigh along the y-direction
// 		vec4 tex001 = texture(mySampler, vec3(h0.x, h0.y, h1.z));
// 		vec4 tex101 = texture(mySampler, vec3(h1.x, h0.y, h1.z));
// 		tex001 = mix(tex101, tex001, g0.x);  //weigh along the x-direction
// 		vec4 tex011 = texture(mySampler, vec3(h0.x, h1.y, h1.z));
// 		vec4 tex111 = texture(mySampler, h1);
// 		tex011 = mix(tex111, tex011, g0.x);  //weigh along the x-direction
// 		tex001 = mix(tex011, tex001, g0.y);  //weigh along the y-direction

// 		return mix(tex001, tex000, g0.z);  //weigh along the z-direction
// };

// vec4 getTexCubicNearest(sampler3D mySampler, vec3 pos, float vs) {
// 	return tricubicNearest(mySampler,(pos)/vs, vec3(vs) );
// }


