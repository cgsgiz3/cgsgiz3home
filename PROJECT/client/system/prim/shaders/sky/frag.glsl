#version 300 es
/* */          
precision highp float;

out vec4 out_color;
uniform cameraBuffer
{
  vec4 camloc;                    
  vec4 camdir;                    
  vec4 camright;                  
  vec4 camup;                     
  vec4 localTimeglobalDeltaTime;  
};
uniform samplerCube cubemap;
 
void main( void )
{            
  float xp = gl_FragCoord.x * localTimeglobalDeltaTime.z / camright.w - localTimeglobalDeltaTime.z / 2.0;
  float yp = gl_FragCoord.y * localTimeglobalDeltaTime.w / camup.w - localTimeglobalDeltaTime.w / 2.0;
  //float xp = gl_FragCoord.x * localTimeglobalDeltaTime.z / camright.w - localTimeglobalDeltaTime.z / 2.0, yp = gl_FragCoord.y * localTimeglobalDeltaTime.w / camup.w - localTimeglobalDeltaTime.w / 2.0;
  
  vec3 D = normalize(camdir.xyz * camloc.w + camright.xyz * xp + camup.xyz * yp);
  if (localTimeglobalDeltaTime.y == 1.0)
  {
    out_color = texture(cubemap, D);
  }
  else {
    out_color = vec4(0.34, 0.656, 0.121, 1);
  }
}