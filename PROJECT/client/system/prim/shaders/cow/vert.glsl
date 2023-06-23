#version 300 es
#define NUM_OF_MATRIX 1
precision highp float;
in vec3 in_pos;
in vec3 in_normal;
out vec3 pos;
out vec3 normal;
out vec3 time;
out vec3 loc;

uniform frameBuffer
{
  mat4 VP;
  mat4 W[NUM_OF_MATRIX];         
};
uniform cameraBuffer
{
  vec4 camloc; 
  vec4 camat;                    
  vec4 camdir;                    
  vec4 camright;                  
  vec4 camup;                     
  vec4 localTimeglobalDeltaTime;  
};            
void main() 
{                        
  int a = int(gl_InstanceID);
  gl_Position = ((VP * W[a]) * vec4(in_pos, 1));
  pos = in_pos;
  normal = in_normal;
  loc = camloc.xyz;
  time = localTimeglobalDeltaTime.xyz;
}
