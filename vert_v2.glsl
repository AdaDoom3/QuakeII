#version 330 core
layout(location=0)in vec3 p;
layout(location=1)in vec2 t;
layout(location=2)in vec3 p2;
layout(location=3)in vec2 t2;
uniform mat4 m,v,j;
uniform float mix_f;
out vec2 u;
void main(){
vec3 pos=mix(p,p2,mix_f);
gl_Position=j*v*m*vec4(pos,1);
u=t;
}
