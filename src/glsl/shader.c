varying vec3 worldPos;
//varying vec3 normalDir;
varying vec3 vertexCol;
varying vec4 viewportPos;

uniform vec3 cameraPos;
uniform vec3 fogColor;
//uniform vec3 lightDir;
uniform vec2 clipDist;
//uniform vec2 windowSize;
uniform float pageSize;


$

void main() {			
	// Calculate the normal value for this vertex, in world coordinates (multiply by gl_NormalMatrix)
	//vertex_normal = normalize(gl_NormalMatrix * gl_Normal);
	// Calculate the light position for this vertex
	//vertex_light_position = normalize(gl_LightSource[0].position.xyz);
	// Set the front color to the color passed through with glColor*f
	//gl_FrontColor = gl_Color;
	
	// Set the position of the current vertex 
	
	vertexCol = gl_Color.xyz;
	//normalDir = gl_Normal.xyz;
	
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	worldPos = gl_Vertex.xyz;
	viewportPos = gl_Position;
	
}


$



float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main() {

	vec2 viewportScaled = viewportPos.xy/viewportPos.w;
	
	float cameraDist = distance(cameraPos, floor(worldPos));
	float clipLength = (clipDist.y - clipDist.x)/1.1;//sqrt(2.0);
	float fogVal = clamp(cameraDist*1.1/clipLength, 0.0, 1.0);
	fogVal = fogVal*fogVal*fogVal;
	vec3 newFogColor = mix(vec3(1.0,1.0,1.0), fogColor, fogVal);
	
	float thresh;
	
	/*
	vec3 edgeVals = worldPos - floor(worldPos);
	float eMin = 1.0/16.0;
	float eMax = 1.0-eMin;
	thresh = float(edgeVals.x < eMin ) + float(edgeVals.x > eMax ) + float(edgeVals.y < eMin ) + float(edgeVals.y > eMax ) + float(edgeVals.z < eMin ) + float(edgeVals.z > eMax );
	*/
	
	//vec3 newWP = floor(worldPos*4.0);
	//thresh = rand(newWP.xy + rand(newWP.xz))/16.0; // + thresh/50.0;
	
	vec3 finalCol = vertexCol;//-thresh/100.0;
	
	finalCol = mix( finalCol, newFogColor, fogVal);
	
	gl_FragColor = vec4(finalCol, 1.0);
	
}





//
//float lightVal = clamp(dot(normalDir,lightDir),0.5,1.0);
//float lerpval = clamp( abs(  worldP.x - 0.5  ) + abs(  worldP.y - 0.5  ) + abs(  worldP.z - 0.5  ), 0.0, 1.0);
//lerpval = lerpval*lerpval;
//vec3 fCol2 = vec3(100.0/255.0,  160.0/255.0,  60.0/255.0);
//vec3 fCol1 = vec3(160.0/255.0, 160.0/255.0,  160.0/255.0);
//vec3 worldP = ((worldPos/128.0)-floor(worldPos/128.0));
//vec3 finalCol = mix(fCol1,fCol2,float(vertexCol.x < 0.4));
//vec3 finalCol = (worldPos/pageSize)-floor(worldPos/pageSize);
//vec3 edgeVals = worldPos - floor(worldPos);
//float eMin = 0.1;
//float eMax = 1.0-eMin;
//float thresh = float(edgeVals.x < eMin ) + float(edgeVals.x > eMax ) + float(edgeVals.y < eMin ) + float(edgeVals.y > eMax ) + float(edgeVals.z < eMin ) + float(edgeVals.z > eMax );
//thresh = thresh/50.0;
//finalCol = finalCol - thresh;