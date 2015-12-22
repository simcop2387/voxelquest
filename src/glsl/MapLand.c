




// vec2 getTerVal(vec3 pos, float camParam) {
	
// 	float blocksPerWorld = 256.0;
	
// 	float res = 
// 		trilin(Texture14, pos/cellsPerWorld, vec3(blocksPerWorld), vec3(1.0/blocksPerWorld));
// 		//getTexCubic(Texture14, pos, cellsPerWorld).r;
	
// 	return vec2(res*0.5,0.0);
// }

float flatten(
	float res,
	vec3 pos,
	vec3 flatCoord,
	float innerRad,
	float outerRad,
	float amount
) {
	
	float flatSpotDis = distance(pos.xy, flatCoord.xy);
	float flatAmount = clamp(
		(outerRad-flatSpotDis) / (outerRad-innerRad),
		0.0,
		1.0
	);
	float newRes = mix(res, pos.z-flatCoord.z, flatAmount*amount);
	// float newRes = mix(res, pos.z – flatCoord.z, flatAmount);  
	
	// return newRes;
	
	return newRes;
}

// float sdCylinder( vec3 p, vec2 h )
// {
//   vec2 d = abs(vec2(length(p.xz),p.y)) - h;
//   return min(max(d.x,d.y),0.0) + length(max(d,0.0));
// }

float sdCylinder(vec3 p, vec2 h) { //h.x = rad, h.y = height
	vec2 d = abs(vec2(length(p.xy),p.z)) - h;
	return min(max(d.x,d.y),0.0) + length(max(d,vec2(0.0)));
}

float sdCone( vec3 p, vec3 c )
{
    vec2 q = vec2( length(p.xy), p.z );
    float d1 = -p.z-c.z;
    float d2 = max( dot(q,c.xy), p.z);
    return length(max(vec2(d1,d2),0.0)) + min(max(d1,d2), 0.0);
}


// float sdCone( vec3 p, vec3 c )
// {
//     vec2 q = vec2( length(p.xz), p.y );
//     float d1 = -p.y-c.z;
//     float d2 = max( dot(q,c.xy), p.y);
//     return length(max(vec2(d1,d2),0.0)) + min(max(d1,d2), 0.0);
// }

// float sdCone( in vec3 p, in vec3 c )
// {
//     vec2 q = vec2( length(p.xz), p.y );
//     float d1 = -q.y-c.z;
//     float d2 = max( dot(q,c.xy), q.y);
//     return length(max(vec2(d1,d2),0.0)) + min(max(d1,d2), 0.);
// }


vec2 getTerVal(vec3 pos, float camParam, bool justLand) {
	float camDis = clamp(camParam,0.0,1.0);
	
	float randVal1 = mod(
							abs(
									sin(pos.z/152.0)*
									sin(pos.x/152.0)*
									sin(pos.y/152.0)
							),
							1.0
					);
	
	float randVal2 = mod(
							abs(
									sin(pos.z/192.0)*
									sin(pos.x/192.0)*
									sin(pos.y/192.0)
							),
							1.0
					);
	
	float randVal3 = mod(
							abs(
									sin(pos.z/64.0)*
									sin(pos.x/64.0)*
									sin(pos.y/64.0)
							),
							1.0
					);
	
	
	randVal1 = clamp((randVal1-0.5)*2.0+1.0+(clamp((pos-3192.0)/4096.0,0.0,1.0)*1.0),0.0,1.0);
	randVal2 = clamp((randVal2-0.5)*2.0+1.0,0.0,1.0);
	randVal3 = clamp((randVal3-0.5)*2.0+1.0,0.0,1.0);
	
	
	
	float camDis0 = clamp(1.0-camDis*1.0,0.0,1.0);
	float camDis1 = clamp(1.0-camDis*3.0,0.0,1.0)*(randVal1);
	float camDis2 = clamp(1.0-camDis*32.0,0.0,1.0)*(randVal2);
	float camDis3 = clamp(1.0-camDis*128.0,0.0,1.0)*(randVal3);
	
	float res = 0.0;
	
	//res = getLand3D2( pos, (+samp2.g*0.5)/1.5 )*64.0 + th;
	//res = opI(res,((1.0-samp2.r)-0.85)*32.0);
	
	//vec4 trilin(in sampler3D t, in vec3 uvIn, in vec3 textureSize, in vec3 texelSize)
	
	float lv0 = 0.0;
	
	vec4 samp0 = vec4(0.0);
	vec4 samp1 = vec4(0.0);
	vec4 samp2 = vec4(0.0);
	vec4 samp3 = vec4(0.0);
	
	vec2 newTC = (pos.xy)/cellsPerWorld;
	vec2 newTC2 = (pos.xy+pos.z)/cellsPerWorld;
	
	
	vec2 th = getTerHeight(Texture2, newTC, newTC2, pos.z);
	
	float baseHeight = (pos.z-th.x);
	
	
	//th.x *= 0.5;
	
	float thMod = 0.0;
	
	//th.x *= 0.25;//clamp(pow(th.x/512.0,8.0),0.25,0.5);
	
	float texScale = 1.0;
	
	th.x *= 0.25;
	
	
	res = th.x; // + lv3*4.0  //
	
	//res *= 0.25;
	
	
	//res = opI(res, (lv2*64.0 + lv3*64.0)*4.0);
	
	vec2 mp = vec2(1.0/mapPitch);
	
	//if (camDis0 > 0.0) {	
		//samp0 = trilin(Texture13, pos*8.0/(cellsPerWorld), vec3(voroSize),1.0/vec3(voroSize));
		samp0 = getTexLin( //trilin(
			Texture13,
			pos*texScale*vec3(0.05,0.05,0.2), // *8.0/(cellsPerWorld),
			voroSize
			//vec3(voroSize),
			//1.0/vec3(voroSize)
		);
		
		lv0 = mix(1.0,-1.0,samp0.g);
		
		// res = opD(
		// 	res,
		// 	lv0*1024.0 * camDis0 * clamp(
		// 		1.0-distance(pos.z,getSeaLevelBase())/1536.0,
		// 		0.0,
		// 		1.0	
		// 	)
		// );
		
		res = opD(
			res,
			-lv0*256.0 *  pow( //camDis0 *
			
			clamp(
							1.0-distance(pos.z,getSeaLevelBase())/2048.0,
							0.0,
							1.0	
						),
			2.0
				
			)
		);
		
	//}
	
	if (camDis1 > 0.0) {
		samp1 = getTexLin(Texture13, pos*texScale*(vec3(0.25,0.25,0.0625)*0.5+randVal1*0.002), voroSize);
		//samp1 = trilin(Texture13, pos*vec3(128.0,128.0,32.0)/(cellsPerWorld), vec3(voroSize),1.0/vec3(voroSize));
		//globTexTap += 1.0;
		
	}
	
	
	
	if (camDis2 > 0.0) {
		samp2 = getTexLin(Texture13, pos*texScale*(1.0+randVal2*0.0125)*0.5, voroSize);
		//globTexTap += 1.0;
	}
	if (camDis3 > 0.0) {
		samp3 = getTexLin(Texture13, pos*texScale*(6.0+randVal3*0.025)*0.5, voroSize);
		//globTexTap += 1.0;
	}
	
	
	float lv1 = mix(1.0,-1.0,samp1.g);
	float lv2 = mix(1.0,-1.0,samp2.g);
	float lv3 = mix(1.0,-1.0,samp3.g);
	
	
	
	
	float oldRes = res;
	// res = opI(res,lv0*1024.0);
	
	// res = mix(res,oldRes,0.75);
	
	// res -= 800.0;
	// res += ((clamp(pow(1.0-samp0.r,4.0),0.0,1.0))*1024.0);
	// res = min(res,th.x);
	
	//res = opD(res,-lv0*1024.0);
	
	
	
	
	
	

	
	
	if (camDis1 > 0.0) {
		res = opD(res,(clamp(pow(1.0-samp1.r,8.0),0.0,1.0))*16.0*camDis1/texScale);
		
		//(heightRes+8.0) - (gradVal2.x)*32.0*float((pos.z-cellSize2.z*0.0) < cellVal2.z);
		//res = (res+16.0) - (samp1.r)*32.0*float(samp1.b < 0.5);
	}
	
	
	float flatDiam = 4096.0*2.0;
	vec3 centerPos = pos - opRep(pos,vec3(flatDiam));// + vec3(flatDiam)*0.5;
	float centerH = getTerHeight2(Texture2, centerPos.xy/cellsPerWorld)+512.0;
	centerPos.z = centerH;
	res = flatten(
		res, pos, centerPos, 0.0, flatDiam*0.5, 0.8
	
		*clamp(
			1.0-abs(baseHeight-centerH)/1024.0,
			0.5,
			1.0
		)
	);
	
	
	
	if (camDis2 > 0.0) {
		//thMod = clamp((16.0-res)/128.0,0.0,1.0);
		thMod = mix(1.0-clamp((16.0-res)/32.0,0.0,1.0),0.3,0.5);
		
		res = opD(res,pow(1.0-samp2.r,4.0)*16.0*thMod*camDis2/texScale);
	}
	
	
	
	
	
	
	
	
	
	if (camDis3 > 0.0) {
		//thMod = clamp((4.0-res)/8.0,0.0,1.0);
		thMod = mix(1.0-clamp((8.0-res)/16.0,0.0,1.0),0.5,0.75);
		
		res = opD(
			res,
			max(pow(1.0-samp3.r,4.0)*2.0*thMod,0.1)*camDis3/texScale	
		);
		
		
	}
	
	
	
	//res = (res+128.0) - (samp0.r)*1024.0*smoothstep(0.0,0.5,samp0.b);
	
	
	
	
	
	//res = res + 2.0;
	//res = mix(res,oldRes,0.4);
	
	
	
	
	
	// res = mix(res,th.x,0.5);
	
	
	//res = 32.0*(samp2.b-0.5);
	//res = opD(res,4.0);
	
	// res = opI(-1.0, 16.0*(samp3.b-0.5));
	// res = opD(res,0.5);
	
	//res = opI(res,-(1.0-samp3.r)*16.0);
	
	samp1.r = abs(mix(samp1.r,0.5,pow(1.0-camDis1,8.0))-0.5)*2.0;
	samp2.r = abs(mix(samp2.r,0.5,pow(1.0-camDis2,8.0))-0.5)*2.0;
	samp3.r = abs(mix(samp3.r,0.5,pow(1.0-camDis3,8.0))-0.5)*2.0;
	
	float finalSamp = samp1.r*samp2.r*samp3.r;//max(max(samp1.r,samp2.r),samp3.r);
	float snowVal = pow(finalSamp,8.0)*0.05;
	
	
	/*
	vec3 opRep( vec3 p, vec3 c )
	{
		return mod(p,c)-0.5*c;
	}
	*/
	
	if (justLand) {
		
	}
	else {
		vec3 cenPoint = vec3(320.0,320.0,420.0);
		vec3 repPos = opRep(pos,cenPoint);
		vec3 repBeg = repPos + 0.5*cenPoint;
		vec3 newPos = (pos-repPos);
		
		float texCen = texture(Texture14, newPos/cellsPerWorld).r;
		float texBel = texture(Texture14,
			(
				newPos + vec3(0.0,0.0,-100.0)	
			)/cellsPerWorld
		).r;
		
		if (
			(texCen > 50.0) &&
			(texBel < -10.0)
		) { //(res > -5.0) && (res < 40.0)) {
			res = min(
				res,
				sdCylinder(repPos, vec2(10.0,200.0))	
			);
			
			res = min(
				res,
				sdCone(repPos,vec3(40.0,80.0,40.0))*0.5	
			);
		}
	}
	
	
	
	
	

	// float blocksPerWorld = 256.0;
	// res = max(
	// 	res,
	// 	trilin(Texture14, pos/cellsPerWorld, vec3(blocksPerWorld), vec3(1.0/blocksPerWorld)).r
	// );
		//getTexCubic(Texture14, pos, cellsPerWorld).r;
	

	return vec2(res,snowVal);
}

