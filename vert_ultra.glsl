#version 330 core
layout(location=0)in vec3 p;
layout(location=1)in vec2 t;
layout(location=2)in float i;
uniform mat4 m,v,j;
out vec2 u;
flat out float ti;
void main(){gl_Position=j*v*m*vec4(p,1);u=t;ti=i;}
