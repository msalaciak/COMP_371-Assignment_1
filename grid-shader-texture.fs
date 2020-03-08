//  A1_29644490
// COMP 371 Assignment 1
// Created by Matthew Salaciak 29644490.



#version 330 core

in vec3 vertexColor;
in vec2 vertexUV;
uniform sampler2D textureSampler
out vec4 FragColor;
uniform vec3 color;
void main()
{
//FragColor = vec4(0.56f, 0.45f, 0.13f, 1.0f);
vec4 textureColor = texture( textureSampler, vertexUV )
    Fragcolor = textureColor;
}

