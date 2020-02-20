#version 330 core


in vec3 vertexColor;


out vec3 color;

uniform vec3 xyz_color;


void main(){
    color = xyz_color;
}
