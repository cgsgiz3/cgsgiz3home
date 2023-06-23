#version 300 es
/* */

precision highp float;
out vec4 out_color;
in vec3 pos;
in vec3 normal;
in vec3 time;
in vec3 loc;

uniform materialBuffer
{
  vec4 KaTrans;
  vec4 Kd1;
  vec4 KsPh;
}; 
vec3 shade() 
{            
  vec3 Ka = KaTrans.xyz / 2.2;
  vec3 Kd = Kd1.xyz / 2.2;
  vec3 Ks = KsPh.xyz / 2.2;
  float Ph = KsPh.w / 2.2;
  float Trans = KaTrans.w / 2.2;
  vec3 L = normalize(vec3(5.0 * sin(time.x), 5, 5));
  vec3 LC = vec3(1, 1, 1);
  vec3 V = normalize(pos - loc.xyz);
  vec3 N = normal;//vec3(0, 1, 0);
  //N = faceforward(N, V, N);
  return vec3(min(vec3(0.1), Ka) +
          max(0.0, dot(N, L)) * Kd  * LC +
          pow(max(0.0, dot(reflect(V, N), L)), Ph) * Ks * LC) * 2.2;   
}
void main()
{
  out_color = vec4(shade(), 1);
  //out_color = vec4(1, 1, 0, 1);
}