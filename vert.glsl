#version 330 core
layout(location=0)in vec3 p;
layout(location=1)in vec2 t;
layout(location=2)in vec2 l;
layout(location=3)in vec3 n;
uniform mat4 m,v,j;
out vec2 u,w;
out vec3 f,q;
out vec4 c;
void main(){gl_Position=j*v*m*vec4(p,1);u=t;w=l;q=n;f=(m*vec4(p,1)).xyz;c=gl_Position;}
