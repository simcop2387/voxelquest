varying vec3 worldPos;
varying vec4 viewportPos;
uniform vec2 windowSize;
uniform vec2 resolution;

uniform sampler2D Texture0; // chars
uniform sampler2D Texture1; // icons
uniform sampler2D Texture2; // background

uniform float passNum;

//tex coords, position
varying vec4 TexCoord0;

//dimensions, border, cornerRad
varying vec4 TexCoord1;

//bg/fg/tg
varying vec4 TexCoord2;
varying vec4 TexCoord3;
varying vec4 TexCoord4;

//unused
varying vec4 TexCoord5;

//border col
varying vec4 TexCoord6;

//roundness,value, isHSL
varying vec4 TexCoord7;


$


void main() {
	
	TexCoord0 = gl_MultiTexCoord0;
	TexCoord1 = gl_MultiTexCoord1;
	TexCoord2 = gl_MultiTexCoord2;
	TexCoord3 = gl_MultiTexCoord3;
	TexCoord4 = gl_MultiTexCoord4;
	TexCoord5 = gl_MultiTexCoord5;
	TexCoord6 = gl_MultiTexCoord6;
	TexCoord7 = gl_MultiTexCoord7;

	gl_Position = gl_Vertex;
	worldPos = gl_Vertex.xyz;
	viewportPos = gl_Position;
	
}


$

vec3 hsv2rgb(vec3 c)
{
	vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
	vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
	return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}


void main() {
	
	vec3 wp = worldPos;
	vec2 viewportScaled = viewportPos.xy/viewportPos.w;
	
	vec4 placeholder1 = TexCoord4;
	vec4 placeholder2 = TexCoord5;
	
	float cornerRad = TexCoord1.w;
	float borderThresh = 1.0-(TexCoord1.z/cornerRad);
	vec2 cornerDis = (1.0-abs(TexCoord0.zw - 0.5)*2.0);
	vec2 cornerDisScaled = vec2(cornerDis.x*TexCoord1.x*0.5/cornerRad, cornerDis.y*TexCoord1.y*0.5/cornerRad) ;
	
	cornerDisScaled = clamp(1.0-cornerDisScaled,0.0,1.0);

	float resVal = max(cornerDisScaled.x,cornerDisScaled.y);
	float resVal2 = resVal;

	if (cornerDisScaled.x > 0.0 && cornerDisScaled.y > 0.0) {
		resVal2 = length(cornerDisScaled.xy);
	}

	float roundness = TexCoord7.z;

	resVal = mix(resVal,resVal2,roundness);

	vec4 oneVec = vec4(1.0);

	vec4 texel0 = texture2D(Texture0, TexCoord0.xy);
	vec4 texel1 = texture2D(Texture1, TexCoord0.xy);
	
	bool isIcon = TexCoord5.x != 0.0;
	bool isShadow = TexCoord5.y != 0.0;
	
	float yMin = TexCoord5.z;
	float yMax = TexCoord5.w;
	
	float alphaMult = 1.0;
	
	if (worldPos.y > yMin) {
		alphaMult = 1.0-clamp((worldPos.y - yMin)*resolution.y/8.0,0.0,1.0);
	}
	if (worldPos.y < yMax) {
		alphaMult = 1.0-clamp((yMax - worldPos.y)*resolution.y/8.0,0.0,1.0);
	}
	
	bool isHSL = bool(TexCoord7.w);
	
	vec4 shadowCol = vec4(0.0,0.0,0.0,0.5*alphaMult);
	texel1.a *= alphaMult;
	
	if (isIcon) {
		
		if (dot(texel1.rgb,oneVec.rgb) == 0.0) {
			discard;
		}
		
		if (isShadow) {
			gl_FragColor = shadowCol;
		}
		else {
			gl_FragColor = texel1;
		}
		
		
		
		
		return;
	}
	
	vec4 bgcol = TexCoord2;
	vec4 fgcol = TexCoord3;

	vec2 value = TexCoord7.xy;

	if (value.x > TexCoord0.z) {
		bgcol = TexCoord4;
	}
	
	value.y = 1.0-value.y;


	float alphaVal = float(resVal < 1.0);
	bgcol.w *= alphaVal;
	bgcol.xyz *= (1.0-resVal/2.0);

	float bval = float(resVal > borderThresh);

	vec4 borderCol = TexCoord6;
	borderCol.w *= alphaVal;
	bval *= borderCol.w;

	float disVal = 0.0;

	float tc[3];
	tc[0] = TexCoord0.z;
	tc[1] = 1.0-TexCoord0.w;
	tc[2] = 1.0;
	
	float tc2[3];
	tc2[0] = value.x;
	tc2[1] = 1.0-value.y;
	tc2[2] = 1.0;
	
	int tcInd = 0;
	vec3 hsvRes = TexCoord2.rgb;
	vec3 hsvRes2 = TexCoord2.rgb;

	if (isHSL) {
		
		if (hsvRes.r < 0.0) {
			hsvRes.r = tc[tcInd];
			hsvRes2.r = tc2[tcInd];
			tcInd++;
		}
		if (hsvRes.g < 0.0) {
			hsvRes.g = tc[tcInd];
			hsvRes2.g = tc2[tcInd];
			tcInd++;
		}
		if (hsvRes.b < 0.0) {
			hsvRes.b = tc[tcInd];
			hsvRes2.b = tc2[tcInd];
			tcInd++;
		}
		
		bgcol.rgb = hsv2rgb(hsvRes);
		
		if (tcInd != 2) {
			bgcol.rgb = mix(bgcol.rgb, bgcol.rgb*0.25, TexCoord0.w);
		}
		
		if (bgcol.a > 0.0) {
			bgcol.a = 1.0;
		}
		
		if (tcInd == 1) {
			if ( 
				(distance(value.x,TexCoord0.z) < 0.25*abs(TexCoord0.w-0.5)) &&
				(TexCoord0.w > 0.5)
			) {
				bgcol.rgb += 0.5;
			}
		}
		
		if (tcInd == 2) {
			
			disVal = distance(value.xy,TexCoord0.zw);
			
			if ((disVal > 0.375)&&(disVal < 0.625)) {
				bgcol.rgb = hsv2rgb(hsvRes2);//0.5;
			}
			
			if (disVal < 0.03125) {
				bgcol.rgb += 0.25;
			}
		}
		
		
	}

	

	bgcol = mix(bgcol,borderCol,bval);


	vec4 resultCol = mix(bgcol,fgcol,texel0.x);
	vec2 bgCoords = (wp.xy+1.0)/2.0;
	

	if (passNum == 0.0) {
		bgCoords += (pow(TexCoord0.zw-vec2(0.5,0.25),vec2(3.0)) )*0.05;
		resultCol = texture2D(Texture2,
			((bgCoords-0.5)*2.0+1.0)/2.0
		);
		resultCol.w = 1.0;
	}
	else {
		//resultCol.xyz += pow(clamp( 1.0-distance(TexCoord0.w,0.25), 0.0, 1.0),3.0)*0.3;
	}
	

	if (isShadow) {
		resultCol.rgb = shadowCol.rgb;
		if (resultCol.a > 0.0) {
			resultCol.a = shadowCol.a;
		}
	}
	
	


	resultCol.a *= alphaMult;

	gl_FragColor = resultCol;


}
