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
void main(){vec4 t=texture(d,vec3(u,ti)),l=texture(e,w);vec3 r=f-a;float ds=length(r),lt=max(0.0,1.0-ds/1000.0)*5.0;float lm=max(l.r,max(l.g,l.b))*6.0;o=vec4(t.rgb*clamp(lm+lt,8.0,20.0),1);}
