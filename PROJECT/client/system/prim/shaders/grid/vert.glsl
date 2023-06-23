#version 300 es
#define NUM_OF_MATRIX 1
/* */
precision highp float;
in vec3 in_pos;
in vec3 in_normal;
out vec3 pos;
out vec3 normal;

uniform frameBuffer
{
  mat4 VP;    
  mat4 W[NUM_OF_MATRIX];
};
void main() 
{                                
  gl_Position = ((VP * W[0]) * vec4(in_pos, 1));
  pos = in_pos;
  normal = in_normal;
}