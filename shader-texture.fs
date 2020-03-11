//  A1_29644490
// COMP 371 Assignment 1
// Created by Matthew Salaciak 29644490.



#version 330 core

in vec3 vertexColor;
in vec2 vertexUV;
in vec3 Normal;
in vec3 FragPos;
out vec4 FragColor;
uniform sampler2D textureSampler;
uniform vec3 color;
uniform  bool textureOn;
uniform vec3 lightColor;
uniform vec3 lightPos;



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

