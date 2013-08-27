varying vec3 worldPos;
varying vec4 viewportPos;
uniform vec2 windowSize;
//uniform vec4 fgColor;
//uniform vec4 bgColor;

uniform sampler2D Texture0;

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

//roundness,value
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

	float roundness = TexCoord7.x;

	resVal = mix(resVal,resVal2,roundness);


	vec4 texel0 = texture2D(Texture0, TexCoord0.xy);
	
	vec4 bgcol = TexCoord2;//mix(TexCoord2, TexCoord3, TexCoord0.w);
	vec4 fgcol = TexCoord3;//mix(TexCoord4, TexCoord5, TexCoord0.w);

	float value = TexCoord7.y;

	if (value > TexCoord0.z) {
		bgcol = TexCoord4;
	}


	float alphaVal = float(resVal < 1.0);
	bgcol.w *= alphaVal;
	bgcol.xyz *= (1.0-resVal/2.0);

	float bval = float(resVal > borderThresh);

	vec4 borderCol = TexCoord6;
	borderCol.w *= alphaVal;
	bval *= borderCol.w;

	bgcol = mix(bgcol,borderCol,bval);


	vec4 resultCol = mix(bgcol,fgcol,texel0.x);

	

	

	gl_FragColor = resultCol;


}