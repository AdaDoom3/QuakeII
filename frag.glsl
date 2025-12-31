#version 330 core
in vec3 c;
in vec2 uv;
out vec4 f;
uniform sampler2D tex;
void main(){
f=vec4(c*texture(tex,uv).rgb,1);
}
