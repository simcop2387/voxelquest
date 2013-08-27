
////////////////  MapShader  ////////////////

uniform vec4 u_OffsetZoom;

//varying vec2 TexCoord0;
varying vec4 v_Color;

$

void main() {
    //TexCoord0 = gl_MultiTexCoord0.xy;

    vec4 newPos = gl_Vertex;
    newPos.xy *= u_OffsetZoom.z;
    newPos.x += u_OffsetZoom.x*u_OffsetZoom.z;
    newPos.y += u_OffsetZoom.y*u_OffsetZoom.z;

    gl_Position = newPos;
    v_Color = gl_Color;

}

$

void main() {

    gl_FragColor = v_Color;
}

