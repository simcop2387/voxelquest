uniform sampler2D u_Texture0;
uniform sampler2D u_Texture1;
uniform sampler2D u_Texture2;
uniform sampler2D u_Texture3;
uniform float u_Time;

uniform vec2 u_Resolution;
uniform vec2 u_TexResolution;
uniform float u_Zoom;
//uniform vec4 u_SourceRect;
//uniform vec4 u_DestRect;


varying vec2 v_TexCoords;

$



$

void main()	{

	vec2 a_TexCoords = uv.xy;
	v_TexCoords = uv.xy;
	gl_Position = vec4( position.xyz, 1.0 );

}

$

void main()	{

	vec4 baseval = texture2D( u_Texture0, v_TexCoords );
	vec4 baseval2 = texture2D( u_Texture1, v_TexCoords );

	vec4 samp0;
	vec4 samp1;
	vec2 tc;

	int i;
	int j;
	float fi;
	float fj;
	float dis;

	int loopMax = 5;
	float fLoopMax = float(loopMax);

	float divVal = 2.0*fLoopMax*fLoopMax;
	float totDis = 0.0;
	float totDiv = 0.0;

	float avgVal = 0.0;

	float minVal = 1.0;

	float oldMinVal = 0.0;
	float mixVal;

	vec2 minVec = vec2(0.0,0.0);
	vec2 totVec = vec2(0.0,0.0);


	float sampDis;
	vec2 size;
	vec2 off0;
	vec2 off1;
	vec2 off2;
	vec2 off3;

    float s11 = 0.0;
    float s01 = 0.0;
    float s21 = 0.0;
    float s10 = 0.0;
    float s12 = 0.0;
	

	for (i = 1; i < 4; i++) {
		sampDis = float(i);

		size = vec2(2.0,0.0);
		off0 = vec2(-sampDis/u_TexResolution.x,0.0);
		off1 = vec2(sampDis/u_TexResolution.x,0.0);
		off2 = vec2(0.0,-sampDis/u_TexResolution.y);
		off3 = vec2(0.0,sampDis/u_TexResolution.y);

	    s11 += texture2D(u_Texture1, v_TexCoords).b;
	    s01 += texture2D(u_Texture1, v_TexCoords + off0).b;
	    s21 += texture2D(u_Texture1, v_TexCoords + off1).b;
	    s10 += texture2D(u_Texture1, v_TexCoords + off2).b;
	    s12 += texture2D(u_Texture1, v_TexCoords + off3).b;

	}

	
    



    vec3 va = normalize(vec3(size.xy,s21-s01));
    vec3 vb = normalize(vec3(size.yx,s12-s10));
    vec3 bump = cross(va,vb);

    bump.b = 0.0;
    bump = normalize(bump);

    bump.g = -bump.g;

    gl_FragColor = vec4((bump.rg+1.0)/2.0, baseval2.b,1.0);

}