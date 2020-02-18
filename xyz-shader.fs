#version 330 core

// Interpolated values from the vertex shaders
in vec3 vertexColor;

// Output data
out vec3 color;

uniform vec3 xyz_color;


void main(){
    color = xyz_color;
}
