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
void main(){vec4 t=texture(d,vec3(u,ti)),l=texture(e,w);vec3 r=f-a;float ds=length(r),fg=1.0-exp(-ds*g*0.2),lt=max(0.0,1.0-ds/1200.0);vec3 lm=l.rgb*6.0+1.2;vec3 cl=pow(t.rgb*lm*(1.0+lt*0.8),vec3(1.4));o=vec4(mix(cl,vec3(0.6,0.7,0.8),fg),1);}
