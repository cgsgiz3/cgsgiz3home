#version 300 es
/* */
precision highp float;
in vec3 in_pos;
in vec3 in_normal;
out vec3 pos;
out vec3 normal;
out vec3 time;
out vec3 loc;

uniform frameBuffer
{
  mat4 VP;     
  mat4 W;
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
float random (vec2 st) {
    return fract(sin(dot(st.xy,
                         vec2(12.9898,78.233)))*
        43758.5453123);
}
void main() 
{
  float a = float(gl_InstanceID);
  float offset = random(vec2(a, a + sin(123.0)));
  float p = floor(a / 20.0);
  a -= p * 20.0;
  vec3 pos = vec3(in_pos.x + (a - 10.0) * 10.0 + a * sin(a + offset) * sin(a * offset * 0.124) + 167.0 * offset * 1.09 - 100.0, in_pos.y, in_pos.z + (p - 20.0) * 10.0 + (a + 20.0) * sin(offset) + 110.0);
  gl_Position = ((VP * W) * vec4(pos, 1));
  pos = in_pos;
  normal = in_normal;
  loc = camloc.xyz;
  time = localTimeglobalDeltaTime.xyz;
}