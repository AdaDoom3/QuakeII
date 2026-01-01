#version 330 core
in vec2 u;
flat in float ti;
uniform sampler2DArray tx;
out vec4 o;
void main(){o=texture(tx,vec3(u,ti));}
