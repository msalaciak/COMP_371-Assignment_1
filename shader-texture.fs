//  A1_29644490
// COMP 371 Assignment 1
// Created by Matthew Salaciak 29644490.



#version 330 core

in vec3 vertexColor;
in vec2 vertexUV;
out vec4 FragColor;
uniform sampler2D textureSampler;
uniform vec3 color;
uniform  bool textureOn;


void main()
{
    
    if(textureOn) {
    vec4 textureColor = texture( textureSampler, vertexUV );
        FragColor = textureColor;
        
    }else
    if(!textureOn) {
        FragColor = vec4(color.r, color.g, color.b, 1.0f);
    }
    
}

