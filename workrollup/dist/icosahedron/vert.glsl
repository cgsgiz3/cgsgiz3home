#version 300 es
/* */
precision highp float;
in vec3 in_pos;
in vec3 in_normal;
uniform float time;
out vec3 pos;
out vec3 normal;

uniform frameBuffer
{
  mat4 WVP;
};
void main() 
{       
  gl_Position = WVP * vec4(in_pos, 1);
  pos = in_pos;
  normal = in_normal;
}