// result fbo
uniform sampler2D Texture0;

// radiosity buffer
uniform sampler2D Texture1;

// combineFBOWithWater
uniform sampler2D Texture2;


uniform bool testOn;

varying vec2 TexCoord0;

$

void main() {

	TexCoord0 = gl_MultiTexCoord0.xy;
	gl_Position = gl_Vertex;
}

$

void main() {

	vec4 oneVec = vec4(1.0);

	vec4 tex0 = texture2D(Texture0, TexCoord0.xy);
	vec4 tex1 = texture2D(Texture1, TexCoord0.xy);
	vec4 tex2 = texture2D(Texture2, TexCoord0.xy);
	//vec4 tex5 = texture2D(Texture5, TexCoord0.xy);
	
	// vec4 matValsGeom = tex5;
	// bool valIsGeom = (dot(matValsGeom.rgb,oneVec.rgb) != 0.0);

	vec3 newRad = tex1.rgb;//pow(tex1.rgb, vec3(2.0) );

	vec3 finalCol = 
		
		//mix(tex0.rgb, tex0.rgb + newRad*0.5, newRad); //tex0.rgb + newRad*0.25;//
		tex0.rgb + tex1.rgb*0.05;//mix(tex0.rgb, tex1.rgb,0.25);
	
	// vec3 resColGS = vec3( clamp(dot(finalCol, oneVec.xyz) / 3.0, 0.0, 1.0) );
	// finalCol = mix(
	// 	resColGS,
	// 	finalCol,
	// 	clamp(1.0 - distance(resColGS, finalCol), 0.0, 1.0) * 1.6
	// );
	
	
	
	
	if (testOn) {
		finalCol = tex1.rgb;
	}
	else {
		//finalCol = mix(finalCol,tex1.rgb,0.5); //finalCol = //tex0.rgb*0.25 + tex1.rgb;
	}
	
	//finalCol = tex1.rgb;
	
	// if (valIsGeom) {
	// 	finalCol.rgb = tex0.rgb;
	// }
	
	
	
	
	gl_FragData[0] = vec4(finalCol.rgb, 1.0);//finalCol




}
