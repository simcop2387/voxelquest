#version 120

// pages fbo
uniform sampler2D Texture0;
uniform sampler2D Texture1;

// prelight fbo 0
uniform sampler2D Texture2;

// prelight fbo 2
uniform sampler2D Texture3;

uniform float clipDist;
uniform float timeOfDay;
uniform vec2 bufferDim;

uniform bool testOn;
uniform float curTime;

uniform vec3 cameraPos;

const int VECS_PER_LIGHT = 4;

uniform int iNumSteps;

float offV[2] = float[](
	0.03125,
	0.125
);

float minRad[1] = float[](
	1.0
);
float maxRad[1] = float[](
	128.0
);

varying vec2 TexCoord0;

$

void main()
{

	TexCoord0 = gl_MultiTexCoord0.xy;
	gl_Position = gl_Vertex;
}

$




void main()
{
	

	vec4 tex0 = texture2D(Texture0, TexCoord0.xy);
	vec4 tex1 = texture2D(Texture1, TexCoord0.xy);
	vec4 tex2 = texture2D(Texture2, TexCoord0.xy);
	vec4 tex3 = texture2D(Texture3, TexCoord0.xy);
	
	vec4 worldPosition = tex0;
	vec4 oneVec = vec4(1.0);
	vec4 lightPosWS = vec4(0.0);
	vec4 samp;
	vec4 wCurPos;
	vec4 testTex = vec4(0.0);
	
	vec3 sStartPos = vec3(TexCoord0.xy,worldPosition.w);
	vec3 sEndPos = vec3(0.0);	
	vec3 sCurPos;
	vec3 resColGS = vec3(0.0);
	vec3 tempVec = vec3(0.0);
	vec3 tempVec2 = vec3(0.0);
	vec3 hsvVal = vec3(0.0);
	vec3 rgbVal = vec3(0.0);
	vec3 bucket = vec3(0.0);
	vec3 myVec = tex1.xyz;
	vec3 lightPosSS = vec3(0.0);
	vec3 lightVec = vec3(0.0);
	vec3 resColor = vec3(0.0);
	vec3 resColorTemp = vec3(0.0);
	
	vec2 offsetCoord = vec2(0.0);
	
	int i;
	int j;
	int k;
	int baseInd;
	float fj;
	float fi;
	float fNumSteps = float(iNumSteps);
	float tot = float(tex1.r + tex1.g + tex1.b + tex1.a > 0.0);
	float totHits = 0.0;
	float baseHeight = worldPosition.w;
	float resComp = 1.0;
	float notBlank = 1.0;
	float colAmount = mix(0.0625,0.25,timeOfDay);
	float flerp = 0.0;
	float totHits2 = 0.0;
	float wasHit = 0.0;
	float ssao = 1.0;
	float totRays;
	float tempv;
	

	


	




	// outlines

	vec2 dirModXY[4];
	dirModXY[0] = vec2(1.0, 0.0);
	dirModXY[1] = vec2(-1.0, 1.0);
	dirModXY[2] = vec2(0.0, 1.0);
	dirModXY[3] = vec2(0.0, -1.0);
	float testHeight = 0.0;
	float bestHeight = 0.0;
	float maxDif = 0.01;


	float pi = 3.14159;



	float curMin = 1.0;
	float curMax = 1.0;
	float curRad = 0.0;
	float hitCount;
	float minRotInc = pi;
	float maxRotInc = 0.01;
	float curRotInc = 0.0;
	float curRot = 0.0;
	float curOff = 0.0;
	
	vec2 newTC = vec2(2.0);

	float minVals = 99999.0;
	float maxVals = 0.0;

	if (tot == 0.0)
	{
		resColor = vec3(0.0);
	}
	else
	{



		for (j = 0; j < 1; j++)
		{
			totHits = 0.0;
			totRays = 0.0;

			curMin = minRad[j];
			curMax = maxRad[j];
			curOff = offV[j];

			for (i = 0; i < iNumSteps; i++)
			{

				fi = float(i) / fNumSteps;

				curRad = mix(curMin, curMax, fi);
				curRotInc = mix(minRotInc, maxRotInc, fi);

				offsetCoord.x = cos(curRot) * curRad;
				offsetCoord.y = sin(curRot) * curRad;

				newTC = TexCoord0.xy + (offsetCoord) / (bufferDim);

				samp = texture2D(Texture2, newTC );
				
				if (samp.w != 0.0) {
					minVals = min(minVals,samp.w);//mix(samp.w,minVals,fi) );
					maxVals = max(maxVals,samp.w);//mix(samp.w,minVals,fi) );
				}
				
				

				curRot += curRotInc;
			}
			
		}
		
		//minVals = mix(minVals,0.5,0.5);
		//maxVals = mix(maxVals,0.5,0.5);
		
		resColor = vec3(
			mix(
				smoothstep(minVals,maxVals,tex3.w),
				tex3.w,
				0.5
			)
		);
		
		//resColor *= tex3.rgb;
		//resColor += tex3.rgb*0.1;

		
		if (testOn) {
			resColor = tex3.rgb;
		}

	}
	
	

	gl_FragData[0] = vec4(resColor.rgb,1.0);
	

}
