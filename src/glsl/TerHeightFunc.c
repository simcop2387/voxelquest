

float getSeaLevelBase() {
	return seaLevel*heightMapMaxInCells;		
}

float getSeaLevel(vec3 pos, float wh, float watVal) {
	return (pos.z - (seaLevel*heightMapMaxInCells-wh))*watVal;		
}


float getTerHeight2(sampler2D mySampler, vec2 newTC) { // , float camDis

	vec2 mp = vec2(1.0/mapPitch);
	vec2 mp2 = vec2(mapPitch);

	return bilin(mySampler, newTC.xy*mapFreqs.x, mp2, mp).r*heightMapMaxInCells;
}

vec2 getTerHeight(sampler2D mySampler, vec2 newTC, vec2 newTC2, float zval) { // , float camDis

	

	vec2 mp = vec2(1.0/mapPitch);
	vec2 mp2 = vec2(mapPitch);
	// vec4 texHM = vec4(
	// 	bicubic(mySampler, newTC.xy*mapFreqs.x*mapPitch, mp2, mp).r,
	// 	bicubic(mySampler, newTC.xy*mapFreqs.y*mapPitch, mp2, mp).r,
	// 	bicubic(mySampler, newTC.xy*mapFreqs.z*mapPitch, mp2, mp).r,
	// 	bicubic(mySampler, newTC.xy*mapFreqs.w*mapPitch, mp2, mp).r
	// );

	vec4 texHM = vec4(
		bilin(mySampler, newTC.xy*mapFreqs.x, mp2, mp).r,
		bilin(mySampler, newTC2.xy*mapFreqs.y, mp2, mp).r,
		bilin(mySampler, newTC2.xy*mapFreqs.z, mp2, mp).r,
		bilin(mySampler, newTC2.xy*mapFreqs.w, mp2, mp).r
	);

	float dotVal = dot(texHM,mapAmps);
	
	
	
	
	
	//dotVal = mix(dotVal,min(dotVal,0.6),0.25);
	
	//float dotVal = bilin(mySampler, newTC.xy, vec2(mapPitch), mp).r;
	

	return vec2(zval-(dotVal*heightMapMaxInCells),dotVal); //-128.0*(1.0-camDis)
}


