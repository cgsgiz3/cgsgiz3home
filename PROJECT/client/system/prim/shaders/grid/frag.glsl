#version 300 es
/* */

precision highp float;
out vec4 out_color;
in vec3 pos;
in vec3 normal;

uniform materialBuffer
{
  vec4 KaTrans;
  vec4 Kd1;
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
vec3 shade() 
{            
  vec3 Ka = KaTrans.xyz / 2.2;
  vec3 Kd = Kd1.xyz / 2.2;
  vec3 Ks = KsPh.xyz / 2.2;
  float Ph = KsPh.w / 2.2;
  float Trans = KaTrans.w / 2.2;
  vec3 L = normalize(vec3(5.0 * sin(localTimeglobalDeltaTime.x), 5, 5));
  vec3 LC = vec3(0.4 * sin(pos.x * 100.0), 0.65 * sin(pos.y), 0.25 * pos.z);
  vec3 V = normalize(pos - camloc.xyz);
  vec3 N = normal;//vec3(0, 1, 0);
  //N = faceforward(N, V, N);
  return vec3(min(vec3(0.1), Ka) +
          max(0.0, dot(N, L)) * Kd  * LC +
          pow(max(0.0, dot(reflect(V, N), L)), Ph) * Ks * LC) * 2.2;   
}
void main()
{
  vec3 color1 = vec3(0.1, 1.0, 0.2);
  vec3 color2 = vec3(1.7, 1.2, 0.0);
  vec3 color3 = vec3(0.9, 0.9, 0.9);
  float col1 = 3.0;
  float col2 = 9.0;
  float h = pos.y;
  vec3 Col = color3;
  if (h < col1)
    Col = mix(color1, color2, h / col1);
  else if (h < col2)
    Col = mix(color2, color3, (h - col1) / (col2 - col1));
  out_color = vec4(Col, 1);
}