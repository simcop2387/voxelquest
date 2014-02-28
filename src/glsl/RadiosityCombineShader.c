// result fbo
uniform sampler2D Texture0;
// result fbo blurred
uniform sampler2D Texture1;

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
    
    vec3 newRad = pow(tex1.rgb,vec3(1.0) );

    vec3 finalCol = mix(tex0.rgb, tex0.rgb+newRad, newRad*0.15);



    vec3 resColGS = vec3( clamp(dot(finalCol,oneVec.xyz)/3.0,0.0,1.0) );
    finalCol = mix(
        resColGS,
        finalCol,
        clamp(1.0-distance(resColGS,finalCol),0.0,1.0)*1.5//*mix(0.5,1.0,totLightDis)
    );

    //finalCol = newRad;
    gl_FragData[0] = vec4(finalCol,1.0);

    
    

}