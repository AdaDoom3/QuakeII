#version 330 core
in vec2 u;
uniform sampler2D tex;
out vec4 o;
void main(){
o=texture(tex,u);
}
