uniform sampler2D u_Texture0;
uniform sampler2D u_Texture1;
uniform sampler2D u_Texture2;
uniform sampler2D u_Texture3;
uniform float u_Time;
uniform float u_Zoom;
uniform vec2 u_Resolution;
uniform vec2 u_MouseCoords;

varying vec2 v_TexCoords;
varying vec2 v_Position;
varying vec2 v_MouseCoords;




$



$

void main()	{

	vec2 a_TexCoords = uv.xy;
	v_TexCoords = uv.xy;
	

	v_Position = position.xy;
	v_MouseCoords = u_MouseCoords;
	
	gl_Position = vec4( position.xyz, 1.0 );

}

$

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main()	{

	vec4 baseval = texture2D( u_Texture0, v_TexCoords );
	vec4 baseval2 = texture2D( u_Texture1, v_TexCoords );
	vec4 idAtPixel = texture2D( u_Texture2, v_TexCoords );

	vec2 newMC = v_MouseCoords;

	newMC.x = (newMC.x+1.0)/2.0;
	newMC.y = (newMC.y+1.0)/2.0;

	vec4 idAtMouse = texture2D( u_Texture2, newMC );

	float lMod = float(idAtMouse.g == idAtPixel.g) * float(idAtPixel.g > 0.0);


	vec3 finalNorm;
	finalNorm.rg = (baseval.rg-0.5)*2.0;
	finalNorm.b = abs(sqrt( abs(1.0-(finalNorm.r*finalNorm.r+finalNorm.g*finalNorm.g) )));
	finalNorm = normalize(finalNorm);
	
	vec3 lightVec;

	vec2 timedLight = v_MouseCoords;
	//timedLight.x += sin(u_Time/50.0)/20.0;
	//timedLight.y += cos(u_Time/100.0)/20.0;

	float disVal = min(1.0-clamp(distance(timedLight, v_Position)*1.0,0.0,1.0)+0.1,1.0);
	lightVec.xy = timedLight - v_Position;
	//lightVec.xy = normalize(lightVec.xy);
	lightVec.z = 50.0/255.0;
	lightVec = normalize(lightVec);
	lightVec.y = -lightVec.y;


	

	///////


	vec4 samp;
	vec3 tc;

	int i;
	int j;
	int k;
	float fi;
	float fj;
	float fk;
	float dis;

	int loopMax = 64;
	int stepAmount = 1;
	float fLoopMax = float(loopMax);

	float totHits = 0.0;
	float totRays = 0.0;
	float bias = 2.0/255.0;

	//vec3 offsetNorm;

	float lerpVal;
	float heightAtCurPos;
	float lightHeight = 60.0/255.0;

	vec3 startPos = vec3(v_MouseCoords.xy,lightHeight);
	vec3 endPos = vec3(v_Position.xy,baseval.b);
	//vec2 offPos = normalize(endPos.xy-startPos.xy);
	//startPos.xy += offPos.yx*0.1;


	vec3 rayPos;
	float lastHit = 1.0;

	for (i = 1; i <= loopMax; i ++) {

		fi = float(i);

		lerpVal = fi/fLoopMax;

		rayPos = mix(startPos,endPos,lerpVal);
		heightAtCurPos = texture2D( u_Texture0, (rayPos.xy+1.0)/2.0 ).b;

		if (heightAtCurPos > rayPos.z+bias) {
			totHits += 1.0;
			lastHit = lerpVal;
		}

		totRays += 1.0;


	}

	float shadVal = clamp((1.0-lastHit*lastHit*lastHit*lastHit) + (1.0-float(totHits>0.0)),0.0,1.0);// 1.0-clamp(float(totHits>0.0)+lastHit,0.0,1.0); //mix(clamp(totHits/totRays,0.0,1.0),float(totHits>0.0),totHits/totRays);

	shadVal = pow(shadVal,0.3);

	//resVal = clamp(pow(resVal,2.0),0.0,1.0);




	float lVal = clamp(dot(finalNorm,lightVec),0.0,1.0)*shadVal;
	lVal = mix(baseval.a*0.3,lVal,lVal);
	lVal *= float(baseval.b > 0.0);

	lVal = sqrt(lVal)*disVal;

	//float v1 = 8.0;
	//float v2 = v1+1.0;
	//lVal = floor(lVal*v1  +(abs(fract(lVal*v1)-0.5)*2.0)*rand(v_TexCoords)     )/v2;   

	float lv2 = sqrt(mix(baseval.a*0.5,lVal,lVal))*0.8;

	//lv2 = pow(lv2,2.0);

	vec3 col0 = vec3(255.0/255.0,153.0/255.0,0.0/255.0)*lVal + lVal*lVal/2.0;
	vec3 col1 = mix(vec3(65.0/255.0,38.0/255.0,16.0/255.0),vec3(241.0/255.0,233.0/255.0,214.0/255.0),lv2)*lv2;

	col1.r += 0.01;


	//vec4(baseval.a,baseval.a,baseval.a,1.0);//

	gl_FragColor = vec4(mix(col0,col1,float(baseval2.w > 0.0)),1.0) + lMod*0.2;//*abs(sin(u_Time/300.0));//vec4(lVal,lVal,lVal,1.0);//vec4(baseval.rgb,lVal);
	

	
}