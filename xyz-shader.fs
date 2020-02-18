#version 330 core

// Interpolated values from the vertex shaders
in vec3 vertexColor;

// Output data
out vec3 color;

uniform vec3 xyz_color;

uniform vec4 color_x;
uniform vec4 color_y;
uniform vec4 color_z;

void main(){
    color = xyz_color;
}
