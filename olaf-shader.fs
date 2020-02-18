#version 330 core

// Interpolated values from the vertex shaders
in vec4 vertexColor;

// Output data
out vec4 color;

uniform vec4 olaf_color;


void main(){
    color = olaf_color;
}

