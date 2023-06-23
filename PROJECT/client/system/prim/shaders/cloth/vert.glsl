#version 300 es
#define NUM_OF_MATRIX 1
#define NUM_OF_CLOTH 60
precision highp float;
in vec3 in_pos;
out vec3 pos;
out vec3 time;
out vec3 loc;

uniform frameBuffer
{
  mat4 VP;
  mat4 W[NUM_OF_MATRIX];         
};                               
uniform clothBuffer
{
  vec4 position[NUM_OF_CLOTH];         
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
  int p = int(gl_VertexID);  
  gl_PointSize = 20.0;
  gl_Position = ((VP * W[a]) * position[a * 4 + p]);
  pos = in_pos;
  loc = camloc.xyz;
  time = localTimeglobalDeltaTime.xyz;
}
