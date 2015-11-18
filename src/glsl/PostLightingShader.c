#version 120

// pages fbo
uniform sampler2D Texture0;
uniform sampler2D Texture1;

// prelight fbo
uniform sampler2D Texture2;
uniform sampler2D Texture3;
uniform sampler2D Texture4;
uniform sampler2D Texture5;

// geom fbo
uniform sampler2D Texture6;
uniform sampler2D Texture7;

// debug fbo
uniform sampler2D Texture8;
uniform sampler2D Texture9;

// pal fbo
uniform sampler3D Texture10;




//uniform float holderMod;
uniform float timeOfDay;
uniform float cellsPerBlock;
uniform vec2 bufferDim;
uniform vec2 mouseCoords;
uniform vec2 resolution;
uniform vec3 lookAtVec;

uniform vec4 worldMarker;
uniform bool markerFound;

uniform bool testOn;
uniform bool gridOn;
uniform float curTime;

uniform mat4 modelviewInverse;
uniform float FOV;
uniform vec3 lightVec;
uniform vec3 lightVecOrig;
uniform vec2 clipDist;


uniform vec3 cameraPos;

const int VECS_PER_LIGHT = 4;

^INCLUDE:MATERIALS^



float offV[2] = float[](
	0.03125,
	0.125
);

float minRad[2] = float[](
	1.0,
	29.0
);
float maxRad[2] = float[](
	32.0,
	255.0
);
vec2 dirModXY[4] = vec2[](
	
	vec2(1.0, 0.0),
	vec2(-1.0, 1.0),
	vec2(0.0, 1.0),
	vec2(0.0, -1.0)
);


varying vec2 TexCoord0;

$

void main()
{

	TexCoord0 = gl_MultiTexCoord0.xy;// = ;//TexCoord0 = gl_MultiTexCoord0;
	gl_Position = gl_Vertex;
}

$



int intMod(int lhs, int rhs)
{
	return lhs - ( (lhs / rhs) * rhs );
}

vec2 pack16(float num)
{

	int iz = int(num);
	int ig = (iz) / 256;
	int ir = iz - ig*256;//intMod(iz, 256);
	

	vec2 res;

	res.r = float(ir) / 255.0;
	res.g = float(ig) / 255.0;

	return res;

}

float unpack16(vec2 num)
{
	return num.r * 255.0 + num.g * 65280.0;
}

float randf(vec2 co)
{
	return fract(sin(dot(co.xy , vec2(12.9898, 78.233))) * 43758.5453);
}


vec3 unpackColor(vec2 num, float lightVal)
{
	return texture3D( Texture10, vec3(lightVal, num.r, num.g + 0.5/255.0) ).rgb;
}

vec3 rgb2hsv(vec3 c)
{
	vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
	vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
	vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));

	float d = q.x - min(q.w, q.y);
	float e = 1.0e-10;
	return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
}


vec3 hsv2rgb(vec3 c)
{
	vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
	vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
	return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

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

vec3 getFogColor()
{
    
    vec3 myRay = getRay();
    
    float zv = pow(1.0-(myRay.z+1.0)/2.0,2.0);
    
    return unpackColor(
        vec2(
            zv,
            TEX_SKY
        ),
        timeOfDay // (-lightVecOrig.z + 1.0)*0.5//timeOfDay
    )
    +
    
    (
        (
            pow(
                clamp(
                    dot(-lightVecOrig,myRay),0.0,1.0
                ),
                16.0    
            )*1.0
            *vec3(1.0,0.5,0.0) //+-lightVecOrig.z*0.25
        )
        +
        (
            pow(
                clamp(
                    dot(-lightVecOrig,myRay),0.0,1.0
                ),
                64.0    
            )*1.0
            *vec3(1.0,1.0,1.0) //+-lightVecOrig.z*0.25
        )    
    )*(pow(clamp((timeOfDay+0.4),0.0,1.0),8.0))
    
    
    +
    
    (
        (
            pow(
                clamp(
                    dot(-lightVecOrig*vec3(1.0,1.0,-1.0),myRay),0.0,1.0
                ),
                64.0    
            )*1.0
            *vec3(0.5,0.5,1.0) //+-lightVecOrig.z*0.25
        )
        +
        (
            pow(
                clamp(
                    dot(-lightVecOrig*vec3(1.0,1.0,-1.0),myRay),0.0,1.0
                ),
                256.0    
            )*1.0
            *vec3(1.0,1.0,1.0) //+-lightVecOrig.z*0.25
        )    
    )*(pow(clamp(1.0-(timeOfDay-0.4),0.0,1.0),8.0))
    ;
    
}


void main()
{
	int i;
	int j;
	int k;
	
	float fj;
	float fi;

	
	
	vec4 oneVec = vec4(1.0);

	vec4 tex0 = texture2D(Texture0, TexCoord0.xy);
	vec4 tex1 = texture2D(Texture1, TexCoord0.xy);
	
	vec4 tex2 = texture2D(Texture2, TexCoord0.xy);
	//vec4 tex3 = texture2D(Texture3, TexCoord0.xy);
	
	vec4 texSpec = texture2D(Texture4, TexCoord0.xy);
	
	// vec4 tex4 = texture2D(Texture6, TexCoord0.xy);
	// vec4 tex5 = texture2D(Texture7, TexCoord0.xy);

	vec4 tex8 = texture2D(Texture8, TexCoord0.xy);
	vec4 tex9 = texture2D(Texture9, TexCoord0.xy);

	float tot = float(tex1.r + tex1.g + tex1.b + tex1.a > 0.0);

	vec4 matVals = vec4(0.0,0.0,pack16(tex1.w));
	
	//vec4 matValsGeom = tex9;
	//bool valIsGeom = (dot(matValsGeom.rgb,oneVec.rgb) != 0.0);

	vec4 worldPosition = tex0;
	vec3 fogCol = getFogColor();
	vec3 resColor = vec3(0.0);
	vec3 rcOrig = vec3(0.0);
	vec3 resColorTemp = vec3(0.0);
	vec3 modColor = vec3(0.0);
	
	float modVal = 0.0;
	
	vec3 tempVec = vec3(0.0);
	vec3 tempVec2 = vec3(0.0);
	vec3 hsvVal = vec3(0.0);
	vec3 hsvVal2 = vec3(0.0);
	vec3 rgbVal = vec3(0.0);
	
	float tempv;
	float newAO = tex2.a;
	float lightRes = max(max(tex2.r,tex2.g),tex2.b);
	lightRes = clamp(newAO*lightRes + lightRes*0.1,0.0,1.0);
	//lightRes = pow(lightRes,2.0);
	
	vec3 myVec = tex1.xyz;
	
	float facingCam = (dot(myVec,lookAtVec) + 1.0)/2.0;
	

	if (tot == 0.0) {
		resColor = fogCol;
	}
	else {
		// resColor = 
		// 	unpackColor(matVals.ba, lightRes ); //(lightRes*0.5+newAO*0.5)
		
		// hsvVal = rgb2hsv(resColor);
		
		// //resColor = mix(resColor*0.5,resColor,lightRes);
		
		
		
		// mix(resColor, vec3(lightRes), abs(lightRes-0.5)*2.0 );
		// 	hsvVal = rgb2hsv(tex2.rgb);
			
			
		// 	resColor =
		// 	mix(
		// 		mix(
		// 				resColor*0.5,
		// 				resColor*(tex2.w*0.5+0.5),
		// 				lightRes//tex2.rgb
		// 		),
		// 		tex2.rgb,
		// 		hsvVal.g*0.5
		// 	);
			
		
		resColor.rgb = vec3(
			unpackColor(matVals.ba,pow(tex2.r,4.0)).r,
			unpackColor(matVals.ba,pow(tex2.g,4.0)).g,
			unpackColor(matVals.ba,pow(tex2.b,4.0)).b
		);
		
		hsvVal = rgb2hsv(tex2.rgb);
		hsvVal2 = rgb2hsv(resColor.rgb);
		hsvVal2.b = hsvVal.b;
		
		resColor.rgb = mix(resColor.rgb,hsv2rgb(hsvVal2),0.25);
		
		// resColor.rgb = mix(
		// 	resColor.rgb,
		// 	pow(tex2.rgb,vec3(0.25)),
		// 	1.0
		// );
	}
	
	
	
	modVal = pow(1.0-lightRes,1.0)*facingCam;
	// modColor = hsv2rgb(vec3(
	// 	mix(0.75, 0.4, modVal)
	// 	,1.0,1.0))*modVal;
	modColor = vec3(0.0,0.65,1.0)*modVal;
	
	modVal = pow(lightRes,10.0)*(1.0-facingCam);
	// modColor += hsv2rgb(vec3(
	// 	mix(0.0, 0.12, modVal)
	// 	,mix(0.75,1.0,1.0-modVal),1.0))*modVal;
	modColor += vec3(1.0,0.5,0.0)*modVal;
	
	
	
	
	if (tot != 0.0)//(false)//
	{

		resColorTemp = resColor;

		tempVec = clamp( (worldPosition.xyz - (cameraPos.xyz)) / 16.0, 0.0, 1.0);

		tempVec2.r = abs(sin( (tempVec.r * 0.25 + tempVec.g * 0.5 + tempVec.b * 0.0) * 4.0 ));
		tempVec2.g = abs(sin( (tempVec.r * 0.0 + tempVec.g * 0.25 + tempVec.b * 0.5) * 4.0 ));
		tempVec2.b = abs(sin( (tempVec.r * 0.5 + tempVec.g * 0.0 + tempVec.b * 0.25) * 4.0 ));

		resColorTemp += 0.25* tempVec2 * 0.2 * (newAO) * (1.0 - timeOfDay); //colAmount * 



		// tempv = 1.0 - dot(resColorTemp.xyz, oneVec.xyz) / 3.0;
		// resColorTemp.r = mix(resColorTemp.r * (newAO + tempv * 0.25), resColorTemp.r, resColorTemp.r * newAO);
		// resColorTemp.g = mix(resColorTemp.g * (newAO + tempv * 0.25), resColorTemp.g, resColorTemp.g * newAO);
		// resColorTemp.b = mix(resColorTemp.b * (newAO + tempv * 0.25), resColorTemp.b, resColorTemp.b * newAO);



		

		// hsvVal = rgb2hsv(resColorTemp);
		// hsvVal.r = (1.0-lightRes);
		// resColorTemp = mix(hsv2rgb(hsvVal),resColorTemp,lightRes);
		
		// hsvVal = rgb2hsv(resColorTemp);
		// hsvVal.b += (lightRes + newAO*0.5) * 0.1 * timeOfDay;
		// //hsvVal.b += lightRes*(1.0-timeOfDay);
		// //hsvVal.g -= resCompTot*(1.0-timeOfDay)*0.25;
		// hsvVal.g = hsvVal.g + 0.6 * (1.0 - lightRes);
		// //hsvVal.b *= mix(resCompTot * 0.25 + 0.25, 1.0, timeOfDay);
		// hsvVal = clamp(hsvVal, 0.0, 1.0);
		// resColorTemp = hsv2rgb(hsvVal);
		
		
		resColorTemp += modColor*mix(
			0.2,
			0.5,
			clamp(1.0-distance(TexCoord0.xy,vec2(0.5)),0.0,1.0)
		)*0.5;
		
		
		//resColorTemp += pow( clamp(lightRes, 0.0, 1.0), 4.0) * (1.0-timeOfDay)*0.5;
		resColorTemp += pow(lightRes,4.0)*0.1;
		
		
		
		
		resColor = resColorTemp;


	}
	
	
	
	resColor = mix(pow(resColor,vec3(1.0)), resColor, timeOfDay);
	
	resColor = mix(resColor*0.5,resColor,tex2.rgb);
	
	resColor += resColor*mix(texSpec.r,1.0,0.25)*texSpec.r;
	
	if (testOn) {
		
		//resColor = vec3(newAO);
		//resColor = vec3(lightRes);//mix(resColor*lightRes,resColor,clamp( (worldPosition.w)*0.5,0.0,1.0) );//vec3(newAO*lightRes + lightRes*0.2);//mix(tex2.a,tex2.a*0.5+0.5,lightRes);
		//resColor = vec3(tex1.w);
	}
	
	//resColor = vec3(newAO);
	//resColor = vec3(lightRes);
	
	
	
	
	float cellSize = 32.0;// *cellsPerBlock;
	vec3 grid0 = 
		//floor(worldPosition.xyz/cellSize);
		abs(mod(worldPosition.xyz, cellSize) - cellSize / 2.0) * 2.0;
	
	float unitBuf = (cellSize - cellSize/32.0);
	
	vec3 gridVec = vec3(
		float(grid0.x >= unitBuf),
		float(grid0.y >= unitBuf),
		float(grid0.z >= unitBuf)	
	);
	
	vec3 absVec = abs(myVec);
	
	vec3 gridVal0 = gridVec;
	
	gridVal0 = max(gridVal0 - abs(lookAtVec),vec3(0.0));
	
	// if (absVec.x > 0.99) {
	// 	gridVal0.x = 1.0;
	// }
	// if (absVec.y > 0.99) {
	// 	gridVal0.y = 1.0;	
	// }
	// if (absVec.z > 0.99) {
	// 	gridVal0.z = 1.0;	
	// }
	
	//gridVal0 *= clamp(1.0-distance(worldPosition.xyz, cameraPos.xyz)/32.0,0.0,1.0);
	
	
	if (!gridOn)
	{
		gridVal0 = vec3(0.0);
	}
	
	// float temp = float(valIsGeom)*(
	// 	0.25 + float(tex8.w < tex0.w)*0.5
	// );
	
	// if (tex8.w > tex0.w) {
	// 	resColor.rgb = mix(resColor.rgb, matValsGeom.rgb, temp);
	// }
	
	
	// if (temp > 0.5) {
		
	// 	tot = 1.0;
		
	// }
	// else {
		resColor.rgb += gridVal0;
	//}
	

	
	float markerDis;
	vec3 dirVec = normalize(worldPosition.xyz-worldMarker.xyz);
	
	
	
	if (markerFound) {
		markerDis = clamp(distance(worldPosition,worldMarker)/(2.0),0.0,1.0);
		
		
		if (
			(markerDis < 1.0) &&
			(markerDis > 0.75)
		) {
			resColor.rgb += 
			
			float(intMod(
				int(
					abs((atan(dirVec.y,dirVec.x))*6.9/3.14159 + curTime/200.0)
				),
				2
			) == 1)*0.5;
		
		}
		
		
	}
	
	float testHeight = 0.0;
	float bestHeight = 0.0;
	vec4 testTex = vec4(0.0);
	for (i = 0; i < 4; i++) {

		testTex = texture2D(Texture0, TexCoord0.xy + dirModXY[i].xy / (bufferDim) );

		testHeight = distance(worldPosition.xyz,testTex.xyz);
		if (testHeight > bestHeight)
		{
			bestHeight = testHeight;
		}
	}
	float outDif = clamp(bestHeight*2.0 / distance(cameraPos,worldPosition.xyz), 0.0, 1.0);
	
	
	outDif *= mix(
		1.0,
		0.0,
		clamp(distance(worldPosition.xyz, cameraPos.xyz)/256.0,0.0,1.0)
	);
	
	
	
	// if (tex0.a == TEX_WATER || tex0.a == TEX_NULL) {
	// 	outDif = 0.0;
	// }
	
	// if (!valIsGeom) {
	// 	//resColor -= outDif*0.5;
	// }
	
	//resColor = vec3(lightRes);
	
	//resColor += sin(worldPosition.x/4.0)*0.1;
	
	//resColor = vec3(outDif);
	
	//resColor = ((myVec+1.0)*0.5);
	
	//resColor = mix(resColor,vec3(resColor.r+resColor.g+resColor.b)/2.0,0.5);
	
	//resColor.rgb = myVec.rgb;
	
	//unpackColor(matVals.ba, lightRes );
	

	//resColor.rgb = tex2.rgb;
	
	//resColor.rgb = texture2D(Texture9, TexCoord0.xy).rgb;

	gl_FragData[0] = vec4(resColor.rgb,tot);
}
