#version 300 es
/* */
precision highp float;
//in vec3 in_normal;
in vec3 in_pos;
in vec2 in_tex;
out vec3 pos;
out vec2 texcoord;

void main() 
{
  gl_Position = vec4(in_pos, 1);
  pos = in_pos;
  texcoord = in_tex;
}