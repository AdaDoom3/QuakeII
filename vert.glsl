#version 330 core
layout(location=0)in vec3 p;
layout(location=1)in vec3 n;
layout(location=2)in vec2 tc;
out vec3 c;
out vec2 uv;
uniform mat4 u;
uniform float t;
void main(){
gl_Position=u*vec4(p,1);
float l=max(dot(n,normalize(vec3(sin(t*0.5),1,cos(t*0.5)))),0.3);
c=vec3(0.7,0.7,0.6)*l;
uv=tc;
}
