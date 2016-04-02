#version 330

// rasterFBO
uniform sampler2D Texture0;
uniform sampler2D Texture1;

// debugTargFBO
uniform sampler2D Texture2;
uniform sampler2D Texture3;

uniform int iNumSteps;
uniform float voxelsPerCell;
uniform int cellsPerHolder;
uniform float FOV;
uniform vec2 clipDist;
uniform vec2 bufferDim;
uniform vec3 cameraPos;
uniform vec3 lightVec;

uniform mat4 modelviewInverse;





$

layout(location = 0) in vec4 vposition;
layout(location = 1) in vec4 data0;

out vec4 worldPos;

void main() {
    
    worldPos = vec4(vposition.xyz,1.0);
    gl_Position = worldPos;
    
}

$

in vec4 worldPos;

layout(location = 0) out vec4 FragColor0;



float offV[2] = float[](
  0.03125*200.0,
  0.125*200.0
);

float minRad[2] = float[](
  1.0,
  1.0
);
float maxRad[2] = float[](
  8.0,
  32.0
);


float calcAO(vec2 texc, vec3 worldPosition, vec3 normVec) {
  
  float totHits = 0.0;
  float totRays = 0.0;

  float curMin = 0.0;
  float curMax = 0.0;
  float curOff = 0.0;
  
  float curRad = 0.0;
  float minRotInc = 3.14159;
  float maxRotInc = 0.01;
  float curRotInc = 0.0;
  float curRot = 0.0;
  
  float curAO = 0.0;
  
  int i;
  int j;
  
  float fi;
  
  vec4 samp;
  
  vec2 offsetCoord = vec2(0.0);
  vec2 newTC = vec2(2.0);

  vec3 testVec = vec3(0.0);

  float camDis2 = distance(worldPosition.xyz,cameraPos.xyz);

  float ssao = 1.0;
  
  float fNumSteps = float(iNumSteps);
  

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

      newTC = texc.xy + (offsetCoord) / (bufferDim);

      // samp1 = texture(Texture0, newTC );
      // samp2 = texture(Texture0, newTC );
      
      samp = texture(Texture0, newTC );//mix(samp1,samp2,float(samp1.w > samp2.w));
      
      testVec = normalize(samp.xyz - (worldPosition.xyz + normVec * curOff));
      curAO = clamp ( clamp(  ( dot(testVec, normVec) ), 0.0, 1.0), 0.0, 1.0)*clamp(
        1.0-distance(worldPosition.xyz,samp.xyz)/camDis2, 0.0, 1.0  
      );
      totHits += curAO;
      
      
      totRays += 1.0;

      curRot += curRotInc;
    }
    
    curAO = clamp(1.0-( totHits / totRays), 0.0, 1.0);
    
    ssao *= curAO;
    
  }
  
  
  
  return clamp(ssao, 0.0, 1.0);
}


void main() {

  vec4 oneVec = vec4(1.0);

  vec2 TexCoord0 = gl_FragCoord.xy/(bufferDim.xy);
  vec4 tex0 = texture(Texture0,TexCoord0.xy);
  vec4 tex1 = texture(Texture1,TexCoord0.xy);
  
  vec4 tex2 = texture(Texture2,TexCoord0.xy);
  vec4 tex3 = texture(Texture3,TexCoord0.xy);


  float lightVal = clamp(dot(tex1.xyz,-lightVec.xyz),0.0,1.0);
  float aoVal = calcAO(TexCoord0, tex0.xyz, tex1.xyz);//tex1.w;
  vec3 finalCol;

  vec3 fogCol = vec3(TexCoord0.y,0.8,1.0);

  float fogDis = clamp((distance(cameraPos,tex0.xyz)*2.0-32.0)/clipDist.y,0.0,1.0);

  if (dot(oneVec,tex0) == 0.0) {
    finalCol = fogCol;
  }
  else {
    finalCol = mix(
      vec3(
        mix(
          (aoVal)*0.25 + lightVal*0.1,lightVal,lightVal  
        ) 
        
        //aoVal 
        
        // (1.0-aoVal*5.0) // 
        // *mod((tex0.xyz+0.01)/float(cellsPerHolder),1.0)
        
      ),
      fogCol,
      fogDis
    );

  }

  if (distance(cameraPos.xyz,tex2.xyz) < distance(cameraPos.xyz,tex0.xyz)) {
    if (dot(tex2.xyz,oneVec.xyz) != 0.0) {
      finalCol = tex3.xyz;
    }
  }
  
  

  //mod(tex.xyz+0.01,1.0);

  FragColor0 = vec4(
    finalCol.xyz,
    1.0
  );

}









