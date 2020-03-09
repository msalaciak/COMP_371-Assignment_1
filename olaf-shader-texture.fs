//  A1_29644490
// COMP 371 Assignment 1
// Created by Matthew Salaciak 29644490.


#version 330 core


in vec3 vertexColor;
in vec2 vertexUV;



out vec4 color;
uniform sampler2D textureSampler;
uniform vec4 olaf_color;


void main(){
     vec4 textureColor = texture( textureSampler, vertexUV );
    color = textureColor;
}

