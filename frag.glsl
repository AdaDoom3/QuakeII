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
void main(){vec4 alb=texture(d,vec3(u,ti));vec3 lm=texture(e,w).rgb;alb.rgb=pow(alb.rgb,vec3(2.2));lm=pow(lm,vec3(2.2))*2.0;vec3 col=alb.rgb*(lm+vec3(0.8));col=(col*(2.51*col+0.03))/(col*(2.43*col+0.59)+0.14);o=vec4(pow(col,vec3(1.0/2.2)),1);}
