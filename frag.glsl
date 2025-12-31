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
void main(){vec4 t=texture(d,vec3(u,ti)),l=texture(e,w);vec3 r=f-a;float ds=length(r),fg=1.0-exp(-ds*g),lt=max(0.0,1.0-ds/500.0);vec3 cl=t.rgb*l.rgb*2.0*(0.5+lt*0.5);o=vec4(mix(cl,vec3(0.5,0.6,0.7),fg),1);}
