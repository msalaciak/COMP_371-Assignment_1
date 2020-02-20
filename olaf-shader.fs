#version 330 core


in vec3 vertexColor;


out vec4 color;

uniform vec4 olaf_color;


void main(){
    color = vec4(olaf_color.r,olaf_color.g,olaf_color.b,olaf_color.a);
}

