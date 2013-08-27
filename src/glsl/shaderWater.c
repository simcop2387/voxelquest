
varying vec3 worldPos;
varying vec3 vertexCol;
varying vec4 viewportPos;

uniform vec3 cameraPos;
uniform vec3 fogColor;
uniform vec2 clipDist;
uniform float pageSize;
uniform float timeVal;

$

void main() {

	vertexCol = gl_Color.xyz;
	
	vec4 mPos = gl_Vertex;

	//mPos.y = mPos.y + sin( (timeVal/1000.0)*mPos.x/1000.0 )*4.0;

	gl_Position = gl_ModelViewProjectionMatrix * mPos;
	worldPos = gl_Vertex.xyz;
	viewportPos = gl_Position;
	
}


$


void main() {

	vec2 viewportScaled = viewportPos.xy/viewportPos.w;
	
	float cameraDist = distance(cameraPos, floor(worldPos));
	float clipLength = (clipDist.y - clipDist.x)/1.1;//sqrt(2.0);
	float fogVal = clamp(cameraDist*1.1/clipLength, 0.0, 1.0);
	fogVal = fogVal*fogVal*fogVal;
	vec3 newFogColor = mix(vec3(1.0,1.0,1.0), fogColor, fogVal);
	vec4 fog4 = vec4(newFogColor,1.0);

	vec4 baseCol = vec4(vertexCol,0.8);
	gl_FragColor = mix( baseCol, fog4, fogVal);
	
}