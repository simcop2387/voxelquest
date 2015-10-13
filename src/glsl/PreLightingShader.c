#version 120

// pages fbo
uniform sampler2D Texture0;
uniform sampler2D Texture1;

// water fbo
uniform sampler2D Texture2;
uniform sampler2D Texture3;

// geom fbo
uniform sampler2D Texture4;
uniform sampler2D Texture5;

// prim depth
uniform sampler2D Texture6;
uniform sampler2D Texture7;

uniform float timeOfDay;
uniform vec2 bufferDim;

uniform bool testOn;

uniform vec3 baseLightVec;

uniform vec2 clipDist;

uniform vec3 lookAtVec;
uniform vec3 cameraPos;
uniform float lightCount;
uniform vec4 lightArr[128];

uniform int vecsPerLight;

uniform int iNumSteps;

float offV[2] = float[](
	0.03125,
	0.125
);

float minRad[2] = float[](
	1.0,
	1.0
);
float maxRad[2] = float[](
	16.0,
	128.0
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

	TexCoord0 = gl_MultiTexCoord0.xy;
	gl_Position = gl_Vertex;
}

$


vec3 getGlobLightCol()
{

	vec3 glCol = vec3(0.0);

	// 0: moon is high, 1: sun is high

	float timeLerp = 0.0;

	if (timeOfDay < 0.5)
	{
		timeLerp = timeOfDay * 2.0;

		glCol = mix(
							vec3(0.1, 0.1, 1.0),
							vec3(1.0, 0.8, 0.7),
							timeLerp
						);
	}
	else
	{
		timeLerp = (timeOfDay - 0.5) * 2.0;

		glCol = mix(
							vec3(1.0, 0.8, 0.7),
							vec3(1.0),
							timeLerp
						);
	}

	return glCol;
}



void main()
{
	

	float specularSolid = 0.0;
	float specularWater = 0.0;

	vec4 tex0 = texture2D(Texture0, TexCoord0.xy);
	vec4 tex1 = texture2D(Texture1, TexCoord0.xy);
	
	vec4 tex2 = texture2D(Texture2, TexCoord0.xy);
	vec4 tex3 = texture2D(Texture3, TexCoord0.xy);
	
	vec4 tex6 = texture2D(Texture6, TexCoord0.xy);
	
	vec4 worldPosition = tex0;
	vec4 waterPosition = tex2;
	vec4 oneVec = vec4(1.0);
	vec4 lightPosWS = vec4(0.0);
	vec4 samp;
	vec4 samp2;
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
	vec3 waterVec = tex3.xyz;
	vec3 lightPosSS = vec3(0.0);
	vec3 lightVec = vec3(0.0);
	vec4 resColor = vec4(0.0);
	
	vec2 offsetCoord = vec2(0.0);
	
	int i;
	int j;
	int k;
	int baseInd;
	float fj;
	float fi;
	float fNumSteps = float(iNumSteps);
	float totSamp = (
		tex1.r + tex1.g + tex1.b + tex1.a +
		tex3.r + tex3.g + tex3.b + tex3.a	
	);
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

	
	float testHeight = 0.0;
	float bestHeight = 0.0;
	float maxDif = 0.01;
	float lightRad = 0.0;
	
	float waterMod;

	float resCompTot = 0.0;
	//float frontLightTot = 0.0;


	float pi = 3.14159;



	float curMin = 1.0;
	float curMax = 1.0;

	float curRad = 0.0;

	float hitCount;

	float minRotInc = pi;
	float maxRotInc = 0.01;
	float curRotInc = 0.0;

	float curRot = 0.0;
	vec2 newTC = vec2(2.0);





	float frontLight = 0.0;
	float frontLightWater = 0.0;
	float backLight = 0.0;
	float topLight = 0.0;
	float bottomLight = 0.0;
	float behindLight = 0.0;
	float lightVal = 0.0;
	float newAO = 0.0;
	float lightRes = 0.0;
	//float lightFlooding = 0.0;

	float lightDis = 0.0;
	float lightDisWater = 0.0;
	float totLightDis = 0.0;

	//float lightColorization = 0.0;
	//float totColorization = 0.0;

	//float lightIntensity = 0.0;
	//float totLightIntensity = 0.0;

	//float totNonColored = 0.0;
	float curAO = 0.0;
	float curOff = 0.0;

	vec3 totLightColor = vec3(0.0);
	vec3 totLightColorWater = vec3(0.0);
	
	
	vec3 curLightColor = vec3(0.0);
	vec3 testVec = vec3(0.0);

	vec3 globDayColor = getGlobLightCol();


	if (totSamp == 0.0)
	{
		resColor = vec4(0.0);
	}
	else
	{



		for (j = 0; j < 2; j++)
		{
			totHits = 0.0;
			totRays = 0.0;

			curMin = minRad[j];
			curMax = maxRad[j];
			curOff = offV[j]/128.0; // todo: change this based on RAY MODE

			for (i = 0; i < iNumSteps; i++)
			{

				fi = float(i) / fNumSteps;

				curRad = mix(curMin, curMax, fi);
				curRotInc = mix(minRotInc, maxRotInc, fi);

				offsetCoord.x = cos(curRot) * curRad;
				offsetCoord.y = sin(curRot) * curRad;

				newTC = TexCoord0.xy + (offsetCoord) / (bufferDim);

				samp = texture2D(Texture0, newTC );
				
				testVec = normalize(samp.xyz - (worldPosition.xyz + myVec * curOff));
				curAO = clamp ( clamp(  ( dot(testVec, myVec) ), 0.0, 1.0), 0.0, 1.0);
				totHits += curAO;
				
				
				totRays += 1.0;

				curRot += curRotInc;
			}
			
			curAO = clamp(1.0-( totHits / totRays), 0.0, 1.0);
			
			ssao *= curAO;
			
		}
		
		
		
		newAO = clamp(ssao, 0.0, 1.0);
		
		//newAO = mix(-0.5, 1.0, newAO);
		//newAO = clamp(newAO, 0.0, 1.0);


		// LIGHT LOOP START
		for (k = 0; k < lightCount; k++)
		{
			baseInd = k * vecsPerLight;
			lightPosWS = lightArr[baseInd + 0];
			lightRad = clipDist.y*2.0;//lightArr[baseInd + 1].w;
			

			lightDis = 1.0 - clamp(distance(worldPosition.xyz, cameraPos.xyz) / lightRad, 0.0, 1.0);
			lightDisWater = 1.0 - clamp(distance(waterPosition.xyz, cameraPos.xyz) / lightRad, 0.0, 1.0);
			

			if ( (lightDis+lightDisWater) != 0.0 ) {
				lightPosSS = lightArr[baseInd + 1].xyz;
				curLightColor = lightArr[baseInd + 2].xyz;


				if (k == 0) {
					lightVec = baseLightVec;
				}
				else {
					lightVec = normalize(lightPosWS.xyz - worldPosition.xyz);
				}
				
				

				sEndPos = lightPosSS;


				//lightIntensity = lightArr[baseInd + 2].w;
				//lightColorization = lightArr[baseInd + 3].r;
				//lightFlooding = lightArr[baseInd + 3].g;

				if (k <= 0)
				{
					curLightColor = globDayColor;
					//lightIntensity = mix(@lightIntensityNight@, 1.0, timeOfDay);
					//lightColorization = mix(@lightColorizationNight@, 0.0, timeOfDay);
					//lightFlooding = mix(@lightFloodingNight@, 0.0, timeOfDay);
				}



				// shadows


				if (k == 0) {
					resComp = tex6.w;
				}
				else {
					totHits = 0.0;
					totHits2 = 0.0;
					totRays = 0.0;
					hitCount = 0.0;
					for (i = 0; i < iNumSteps; i++)
					{
						fi = float(i);
						flerp = (fi / fNumSteps);

						sCurPos = mix(sStartPos, sEndPos, flerp);

						samp = texture2D(Texture0, sCurPos.xy);
						//samp2 = texture2D(Texture4, sCurPos.xy);
						wasHit = float( 
							samp.w
							//max(samp.w,samp2.w)
							> sCurPos.z
						);
						
						totHits += wasHit;
						hitCount += 1.0;
						
						

					}
					resComp = mix(1.0, 0.0, clamp(totHits*2.0/hitCount,0.0,1.0));
					resComp = clamp(pow(resComp,2.0), 0.0, 1.0);
					
				}

				
				


				frontLight = clamp(
					dot(myVec, lightVec),
					0.0,
					1.0
				);
				
				frontLightWater = clamp(
					dot(waterVec, lightVec),
					0.0,
					1.0
				);
				
				specularSolid += pow(clamp( dot(
				 	normalize(reflect( lookAtVec , myVec )),
				 	lightVec
				 ), 0.0, 1.0 ),8.0) * resComp * lightDis;
				specularWater += pow(clamp( dot(
				 	normalize(reflect( lookAtVec , waterVec )),
				 	lightVec
				 ), 0.0, 1.0 ),8.0) * lightDis;
				
				
				backLight = clamp(dot(myVec, -lightVec), 0.0, 1.0);
				
				
				topLight = clamp(dot(myVec, vec3(lightVec.x, lightVec.y, -lightVec.z)), 0.0, 1.0);
				bottomLight = clamp(dot(myVec, vec3(0.0, 0.0, -1.0)), 0.0, 1.0);
				behindLight = clamp(dot(myVec, vec3(-lightVec.x, -lightVec.y, 0.0)), 0.0, 1.0);


				totLightDis += lightDis;
				

				totLightColor += frontLight * resComp * curLightColor * lightDis;
				totLightColor += vec3(0.0, 1.0, 1.0) * colAmount * (bottomLight) * 0.5 * lightDis;
				totLightColor += vec3(0.9, 0.5, 0.2) * colAmount * (timeOfDay * 0.5 + 0.5) * behindLight * 0.5 * lightDis * (1.0 - frontLight);
				
				totLightColorWater += frontLightWater * curLightColor * lightDisWater;
				
				

				resCompTot += (1.0-resComp) * lightDis;
				//frontLightTot += frontLight * lightDis;

			}
		}
		// LIGHT LOOP END

		resColor.xyz = pow(totLightColor.xyz*0.75+newAO*0.25,vec3(0.5));;////mix(newAO*totLightColor.xyz,totLightColor.xyz, max(max(totLightColor.x, totLightColor.y), totLightColor.z) ); // 
		resColor.w = newAO;
	}
	
	

	gl_FragData[0] = resColor;
	gl_FragData[1] = vec4(totLightColorWater,newAO);
	gl_FragData[2] = vec4(specularSolid*0.25+newAO*0.25,specularWater,0.0,0.0);
	gl_FragData[3] = vec4(0.0);

}
