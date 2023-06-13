#version 300 es
/* */

precision highp float;
out vec4 out_color;
uniform sampler2D Tex1;
in vec3 pos;
in vec2 texcoord;
uniform texBuffer
{
  vec4 timetextures;
};

vec2 mul( vec2 A, vec2 B )
{
  vec2 R = vec2(A.x * B.x - A.y * B.y, A.x * B.y + A.y * B.x);
  return R;
}

float lenth1( vec2 A )
{
  return A.x * A.x + A.y * A.y;
}

vec3 Jul( vec2 A, vec2 B )
{
  float n = 0.0;

  while(n < 255.0 && lenth1(A) < 4.0)
  {
    A = mul(A, A) + B;
    n = n + 1.0;
  }
  return vec3(n/255.0,n/8.0/255.0, n*8.0 / 255.0);
}
void main()
{ 
  if (timetextures.z != 0.0) {
    vec4 tc = texture(Tex1, texcoord);
    out_color = vec4(tc.xyz, 1);
  }
  else {
    out_color = vec4(Jul(pos.xy, pos.xy), 1);//vec2(0.35 + 0.08 * sin(time + 3.0), 0.39 + 0.08 * sin(time * 1.1))), 1);
  }
}