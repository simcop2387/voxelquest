#version 120

uniform sampler3D Texture0; //volume texture nearest
uniform sampler3D Texture1; //voro texture linear
uniform sampler2D Texture2; //2d to 3d coords

uniform vec2 resolution;
uniform float unitsPerDim;
uniform vec3 worldMin;
uniform vec3 worldMax;
uniform float threshVal;

varying vec2 TexCoord0;


$

void main() {
    TexCoord0 = gl_MultiTexCoord0.xy;
    gl_Position = gl_Vertex;
}

$


void main() {

	vec3 coords = texture2D(Texture2, TexCoord0.xy).rgb;
	vec3 newCoords = coords;//(coords * 255.0 + 0.5)/256.0;
	vec4 tex1 =  texture3D(Texture0, newCoords);
	vec4 tex2 =  texture3D(Texture1, newCoords);

	vec4 res = vec4(0.0,0.0,0.0,0.0);

	int i = 0;
	int j = 0;
	int k = 0;

	float fi = 0.0;
	float fj = 0.0;
	float fk = 0.0;


	float rad = 1.0/(unitsPerDim);
	vec4 samp =     vec4(0.0,0.0,0.0,0.0);
	vec4 bestSamp = vec4(0.0,0.0,0.0,0.0);
	float curDis = 0.0;


	float notValid = 999.0;

	float minDis1 = notValid;
	float minDis2 = notValid;

	const int minrad = -1;
	const int maxrad = 1;

	vec3 newCoords2 = newCoords;
	//newCoords2.x *= 0.5;
	//newCoords2.z *= 0.5;
	



	gl_FragData[0] = float(tex1.a > threshVal/255.0)*(vec4(1.0,0.0,0.0,2.0/255.0));

}

