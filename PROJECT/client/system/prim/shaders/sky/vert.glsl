#version 300 es
/* */
precision highp float;
in vec3 in_pos;
void main() 
{       
  gl_Position = vec4(in_pos, 1);
}