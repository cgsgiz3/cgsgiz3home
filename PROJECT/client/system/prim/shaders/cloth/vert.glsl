#version 300 es
#define NUM_OF_MATRIX 1
#define NUM_OF_CLOTH 60
precision highp float;
in vec3 in_pos;
out vec3 pos;
out vec3 time;
out vec3 loc;
out vec3 normal;

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
  vec3 N = vec3(1, 1, 1);
  gl_PointSize = 20.0;                    
  if (p == 1)
  {
    N = cross(position[a * 4 + p + 1].xyz - position[a * 4 + p].xyz, position[a * 4 + p + 2].xyz - position[a * 4 + p].xyz);
  } else if (p == 2)
  { 
    N = cross(position[a * 4 + p + 2].xyz - position[a * 4 + p].xyz, position[a * 4 + p - 1].xyz - position[a * 4 + p].xyz);
  } else if (p == 3)
  { 
    N = cross(position[a * 4 + p - 2].xyz - position[a * 4 + p].xyz, position[a * 4 + p + 1].xyz - position[a * 4 + p].xyz);
  } else if (p == 4)
  { 
    N = cross(position[a * 4 + p - 1].xyz - position[a * 4 + p].xyz, position[a * 4 + p - 2].xyz - position[a * 4 + p].xyz);
  }
  //if (p == 3 || p == 4)
  //{           
  //  N += cross(position[a * 4 + p + 1].xyz - position[a * 4 + p].xyz, position[a * 4 + p - 2].xyz - position[a * 4 + p].xyz);
  //}
  gl_Position = ((VP * W[a]) * position[a * 4 + p]);
  pos = in_pos;
  loc = camloc.xyz;
  normal = position[a * 4 + p].xyz;
  time = localTimeglobalDeltaTime.xyz;
}
