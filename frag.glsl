#version 330 core
in vec2 u,w;
in vec3 f,q;
in vec4 c;
flat in float ti;
uniform sampler2DArray d;
uniform sampler2D e;
uniform vec3 a;
uniform float g;
out vec4 o;
void main(){vec4 t=texture(d,vec3(u,ti)),l=texture(e,w);o=vec4(t.rgb*l.rgb*2.0,1);}
