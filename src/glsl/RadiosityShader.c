// pages fbo
uniform sampler2D Texture0;
uniform sampler2D Texture1;

// result fbo
uniform sampler2D Texture2;

// result fbo blurred
//uniform sampler2D Texture3;



uniform vec2 bufferDim;
uniform vec2 resolution;
uniform vec3 cameraPos;
uniform vec3 lightVec;
//uniform vec3 lightPosWS;

varying vec2 TexCoord0;

uniform int iNumSteps;

$

void main() {

	TexCoord0 = gl_MultiTexCoord0.xy;
	gl_Position = gl_Vertex;
}

$

int intMod(int lhs, int rhs) {
	return lhs - ( (lhs / rhs) * rhs );
}

vec2 pack16(float num) {

	int iz = int(num);
	int ir = intMod(iz, 256);
	int ig = (iz) / 256;

	vec2 res;

	res.r = float(ir) / 255.0;
	res.g = float(ig) / 255.0;

	return res;

}

// float unpack16(vec2 num) {
// 	return num.r * 255.0 + num.g * 65280.0;
// }

// float isNotGeom(vec2 num) {
// 	return float ( (unpack16(num)) < 32768.0);
// }


void main() {

	float fNumSteps = float(iNumSteps);
	
	vec4 tex0 = texture2D(Texture0, TexCoord0.xy);
	vec4 tex1 = texture2D(Texture1, TexCoord0.xy);
	vec4 tex2 = texture2D(Texture2, TexCoord0.xy);
	vec3 oneVec = vec3(1.0);
	
	vec4 worldPosition = tex0;//texture2D(Texture4, TexCoord0.xy);

	

	//vec3 lightVec = normalize(lightPosWS.xyz - worldPosition.xyz);



	float totHits = 0.0;
	float totRays = 0.0;
	int i;
	int j;
	float fi;
	float fj;



	float curHeight;
	vec2 offsetCoord = vec2(0.0);
	vec4 samp0 = vec4(0.0);
	vec4 samp1 = vec4(0.0);
	vec4 samp2 = vec4(0.0);
	vec3 newTC = vec3(0.0);
	vec3 newRes = vec3(0.0);

	vec3 baseNormOrig = tex1.xyz;//normalize((tex1.rgb - 0.5) * 2.0);
	//vec3 baseNormRef = normalize(reflect(lightVec, baseNormOrig));
	vec3 testNormOrig = vec3(0.0);
	vec3 testNormRef = vec3(0.0);

	float pi = 3.14159;

	float minRad = 2.0;
	
	float divVal = 1.0-clamp( distance(TexCoord0.xy,vec2(0.5,0.5)) ,0.0,1.0);
	
	
	float maxRad = bufferDim.y * mix(1.0,64.0,tex0.w*divVal) / 128.0;//mix(1.0,4.0,divVal);
	float curRad = 0.0;

	float minRotInc = pi;
	float maxRotInc = 0.01;
	float curRotInc = 0.0;
	float curRot = 0.0;

	float minV = 0.0;
	float maxV = 0.0;
	float curV = 0.0;

	float totMin = 0.0;
	float totMax = 0.0;
	float avgVal = 0.0;

	vec3 minMaxCur = vec3(0.0);
	vec3 avgVec = vec3(0.0);


	vec3 baseVec = vec3(0.0);
	vec3 testVec = vec3(0.0);



	float finalRes = 0.0;
	float disMult = 0.0;
	float weightVal = 0.0;

	for (i = 0; i < iNumSteps; i++) {


		fi = float(i) / fNumSteps;

		curRad = mix(minRad, maxRad, fi);
		curRotInc = mix(minRotInc, maxRotInc, fi);

		offsetCoord.x = cos(curRot) * curRad;
		offsetCoord.y = sin(curRot) * curRad;

		newTC.xy = TexCoord0.xy + offsetCoord / (bufferDim);
		samp0 = texture2D(Texture0, newTC.xy );
		samp1 = texture2D(Texture1, newTC.xy );
		samp2 = texture2D(Texture2, newTC.xy );
		

		testNormOrig = samp1.xyz;//normalize( (samp1.rgb - 0.5) * 2.0 );
		testNormRef = normalize(reflect(-lightVec, testNormOrig));

		disMult = clamp(
			(samp0.w - worldPosition.w)/256.0,
			0.0,
			1.0
		);

		
		weightVal = 1.0-curRad/maxRad;

		newRes += 
			float((tex1.a*samp1.a) != 0.0) *
			//tex2.a * tex1.a * samp2.a * // samp1.a *
			samp2.rgb *
			// disMult *
			clamp(dot(testNormRef, baseNormOrig), 0.0, 1.0) * weightVal;
		
		totRays += weightVal;

		curRot += curRotInc;
	}

	newRes = clamp(newRes / (totRays / 16.0), 0.0, 1.0);
	newRes = pow(newRes, vec3(4.0));
	//newRes = min(newRes,normalize(newRes));
	gl_FragData[0] = vec4(newRes, 1.0); //*divVal







}
