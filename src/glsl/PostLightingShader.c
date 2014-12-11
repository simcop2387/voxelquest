#version 120

// pages fbo
uniform sampler2D Texture0;
uniform sampler2D Texture1;

// prelight fbo
uniform sampler2D Texture2;
uniform sampler2D Texture3;

// geom fbo
uniform sampler2D Texture4;
uniform sampler2D Texture5;

// pal fbo
uniform sampler2D Texture6;


//uniform float holderMod;
uniform float timeOfDay;
uniform float pixelsPerCell;
uniform float blockSizeInCells;
uniform vec2 bufferDim;
uniform vec2 mouseCoords;
uniform vec2 resolution;

uniform vec4 worldMarker;
uniform bool markerFound;

uniform bool testOn;
uniform bool gridOn;
uniform float curTime;

uniform vec3 cameraPos;

const float TEX_GRASS = 12.0 / 255.0;
const float TEX_GRASS2 = 13.0 / 255.0;
const int VECS_PER_LIGHT = 4;

const float TEX_NULL = 0.0;
const float TEX_WATER =     32.0 / 255.0;
const float TEX_METAL =     33.0 / 255.0;
const float TEX_GLASS =     35.0 / 255.0;

const float TEX_PLASTER =   28.0 / 255.0;
const float TEX_MORTAR =  	16.0 / 255.0;

const float TEX_EARTH =   36.0 / 255.0;
const float TEX_BARK =    42.0 / 255.0;


const float TEX_SAND =    14.0 / 255.0;
const float TEX_SAND2 =    15.0 / 255.0;


uniform int iNumSteps;

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
	return texture2D( Texture6, vec2(lightVal, (num.g * 255.0 + num.r + 0.5) / 255.0 ) ).rgb;
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


vec3 getFogColor(float lv)
{
	float lvBase = (lv-0.5)*0.65 + 0.5;
	
	float newlv = clamp( 1.0 - pow( (lvBase - 0.5) * 2.0, 2.0 ), 0.0, 1.0);
	float newlv2 = clamp( 1.0 - pow( (lvBase - 0.5) * 4.0 - 1.0, 2.0 ), 0.0, 1.0);

	vec3 fogColor1 = vec3(0.0);
	vec3 fogColor2 = vec3(0.0);

	// 0: moon is high, 1: sun is high

	float timeLerp = 0.0;

	if (timeOfDay < 0.5)
	{
		timeLerp = timeOfDay * 2.0;

		fogColor1 = mix(
									vec3(0.0, 0.0, 0.05),
									vec3(0.3, 0.1, 0.05),
									timeLerp
								);
		fogColor2 = mix(
									vec3(0.025, 0.0, 0.1),
									vec3(1.0, 0.8, 0.7),
									timeLerp
								);
	}
	else
	{
		timeLerp = (timeOfDay - 0.5) * 2.0;

		fogColor1 = mix(
									vec3(0.3, 0.1, 0.05),
									vec3(0.05, 0.1, 0.3),
									timeLerp
								);
		fogColor2 = mix(
									vec3(1.0, 0.8, 0.7),
									vec3(0.7, 0.8, 1.0),
									timeLerp
								);
	}

	return mix(fogColor1, fogColor2, newlv ) + pow(newlv2, 4.0) * timeLerp / 4.0;
}


void main()
{
	int i;
	int j;
	int k;
	
	float fj;
	float fi;

	float fNumSteps = float(iNumSteps);
	
	
	vec4 oneVec = vec4(1.0);

	vec4 tex0 = texture2D(Texture0, TexCoord0.xy);
	vec4 tex1 = texture2D(Texture1, TexCoord0.xy);
	
	vec4 tex2 = texture2D(Texture2, TexCoord0.xy);
	//vec4 tex3 = texture2D(Texture3, TexCoord0.xy);
	
	vec4 tex4 = texture2D(Texture4, TexCoord0.xy);
	vec4 tex5 = texture2D(Texture5, TexCoord0.xy);

	float tot = float(tex1.r + tex1.g + tex1.b + tex1.a > 0.0);

	vec4 matVals = vec4(0.0,0.0,pack16(tex1.w));
	
	vec4 matValsGeom = tex5;
	bool valIsGeom = (dot(matValsGeom.rgb,oneVec.rgb) != 0.0);

	vec4 worldPosition = tex0;
	vec3 fogCol = getFogColor(TexCoord0.y);
	vec3 resColor = vec3(0.0);
	vec3 resColorTemp = vec3(0.0);
	
	vec3 tempVec = vec3(0.0);
	vec3 tempVec2 = vec3(0.0);
	vec3 hsvVal = vec3(0.0);
	vec3 rgbVal = vec3(0.0);
	
	float tempv;
	float newAO = tex2.a;
	float lightRes = max(max(tex2.r,tex2.g),tex2.b);
	
	vec3 myVec = tex1.xyz;
	

	if (tot == 0.0) {
		resColor = fogCol;
	}
	else {
		resColor = 
			unpackColor(matVals.ba, lightRes );
			
			hsvVal = rgb2hsv(tex2.rgb);
			
			
			resColor =
			mix(
				mix(
						resColor*0.5,
						resColor*(tex2.w*0.5+0.5),
						lightRes//tex2.rgb
				),
				tex2.rgb,
				hsvVal.g*0.5
			);
	}
	
	
	
	
	
	
	
	
	if (tot != 0.0)
	{

		resColorTemp = resColor;

		tempVec = clamp( (worldPosition.xyz - (cameraPos.xyz)) / 2048.0, 0.0, 1.0);

		tempVec2.r = abs(sin( (tempVec.r * 0.25 + tempVec.g * 0.5 + tempVec.b * 0.0) * 4.0 ));
		tempVec2.g = abs(sin( (tempVec.r * 0.0 + tempVec.g * 0.25 + tempVec.b * 0.5) * 4.0 ));
		tempVec2.b = abs(sin( (tempVec.r * 0.5 + tempVec.g * 0.0 + tempVec.b * 0.25) * 4.0 ));

		

		resColorTemp += 0.25* tempVec2 * 0.2 * (newAO) * (1.0 - timeOfDay); //colAmount * 

		tempv = 1.0 - dot(resColorTemp.xyz, oneVec.xyz) / 3.0;
		
		
			resColorTemp.r = mix(resColorTemp.r * (newAO + tempv * 0.25), resColorTemp.r, resColorTemp.r * newAO);
			resColorTemp.g = mix(resColorTemp.g * (newAO + tempv * 0.25), resColorTemp.g, resColorTemp.g * newAO);
			resColorTemp.b = mix(resColorTemp.b * (newAO + tempv * 0.25), resColorTemp.b, resColorTemp.b * newAO);


		
		
		hsvVal = rgb2hsv(resColorTemp);
		hsvVal.b += (lightRes + newAO*0.5) * 0.1 * timeOfDay;
		//hsvVal.b += lightRes*(1.0-timeOfDay);
		//hsvVal.g -= resCompTot*(1.0-timeOfDay)*0.25;
		hsvVal.g = hsvVal.g + 0.6 * (1.0 - lightRes);
		//hsvVal.b *= mix(resCompTot * 0.25 + 0.25, 1.0, timeOfDay);

		
		if (
			(
				(matVals.a >= TEX_EARTH) &&
				(matVals.a <= TEX_BARK)
			) ||
			(
				(matVals.a >= TEX_SAND) &&
				(matVals.a <= TEX_SAND2)
			)
								
		) {
			tempv = (1.0+sin( worldPosition.z/(8000.0) ) )/2.0;  //sin( dot(worldPosition.xyz,oneVec.xyz)/5235.0 )
			
			hsvVal.g += mix(-0.1*lightRes,0.2*lightRes,tempv);
			hsvVal.b -= mix(-0.1*lightRes,0.3*lightRes,tempv);
		}
		
		
		
		hsvVal = clamp(hsvVal, 0.0, 1.0);
		resColorTemp = hsv2rgb(hsvVal);
		
		// resColorTemp = mix(
		// 	mix(resColorTemp*0.25,resColorTemp,pow(newAO,2.0)),
		// 	resColorTemp,
		// 	resComp
		// );

		resColorTemp += pow( clamp(lightRes, 0.0, 1.0), 4.0) * (1.0-timeOfDay)*0.5;
		
		//resColorTemp += (tex2.rgb*0.5 + dot(tex2.rgb,oneVec.rgb)/6.0) * (1.0-timeOfDay) ;
		
		resColorTemp += pow(lightRes,4.0)*0.1;
		
		resColor = resColorTemp;


	}
	
	
	
	
	
	if (testOn) {
		resColor = tex2.rgb;
	}
	
	
	
	
	
	
	float unitSizeInPixels = pixelsPerCell*1.0;//*blockSizeInCells;
	vec3 grid0 = abs(mod(worldPosition.xyz, unitSizeInPixels) - unitSizeInPixels / 2.0) * 2.0;
	float gridVal0 = float(
										 (grid0.x >= (unitSizeInPixels - 4.0)) ||
										 (grid0.y >= (unitSizeInPixels - 4.0))
									 )  * clamp(myVec.z - 0.4, 0.0, 1.0);
	
	//* float(aoval != 0.0)
	
	if (!gridOn)
	{
		gridVal0 = 0.0;
	}
	
	float temp = 0.25 + float(tex4.w > tex0.w)*0.5;
	resColor.rgb = mix(resColor.rgb, matValsGeom.rgb,temp*float(valIsGeom));
	
	if (temp > 0.5) {
		
		tot = 1.0;
		
	}
	else {
		resColor.rb += gridVal0;
	}
	

	
	float markerDis;
	vec3 dirVec = normalize(worldPosition.xyz-worldMarker.xyz);
	
	
	
	if (markerFound) {
		markerDis = clamp(distance(worldPosition,worldMarker)/(2.0*pixelsPerCell),0.0,1.0);
		
		
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
	

	gl_FragData[0] = vec4(resColor.rgb,tot);
}
