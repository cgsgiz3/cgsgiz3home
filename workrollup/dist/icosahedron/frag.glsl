#version 300 es
/* */

precision highp float;
out vec4 out_color;
uniform sampler2D Tex1; 
in vec3 pos;
in vec3 normal;
/*
uniform vec4 loc;
uniform float time;
*/

uniform materialBuffer
{
  vec4 KaTrans;
  vec4 Kd;
  vec4 KsPh;
}; 
uniform cameraBuffer
{
  vec4 camloc;                    
  vec4 camdir;                    
  vec4 camright;                  
  vec4 camup;                     
  vec4 localTimeglobalDeltaTime;  
};
vec3 shade() {            
    vec3 L = normalize(vec3(5.0 * sin(localTimeglobalDeltaTime.x * 10.0), 5, 5));
    vec3 LC = vec3(1, 1, 1);
    vec3 V = normalize(pos - camloc.xyz);
    vec3 N = normal;//vec3(0, 1, 0);
    //N = faceforward(N, V, N);
    return vec3(min(vec3(0.1), KaTrans.xyz) +
            max(0.0, dot(N, L)) * Kd.xyz  * LC +
            pow(max(0.0, dot(reflect(V, N), L)), KsPh.w) * KsPh.xyz * LC);   
}
void main()
{
  vec4 tc1 = texture(Tex1, pos.xy); 
  out_color = vec4(shade(), 1);
}