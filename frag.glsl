#version 330 core
in vec2 u,w;
in vec3 f,q;
in vec4 c;
uniform sampler2D d,e;
uniform vec3 a;
uniform float g;
out vec4 o;
void main(){vec4 x=texture(d,u),y=texture(e,w);vec3 b=normalize(a-f);float h=max(dot(q,b),0.0);vec3 i=x.rgb*y.rgb*(0.3+h*0.7);float z=length(c.xyz)/c.w;i=mix(i,vec3(0.5,0.6,0.7),clamp(z*g,0,1));o=vec4(i,x.a);}
