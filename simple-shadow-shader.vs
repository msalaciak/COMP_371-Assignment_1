//  A2_29644490
// COMP 371 Assignment 2
// Created by Matthew Salaciak 29644490.

#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 lightSpaceMatrix;
uniform mat4 mvp;

void main()
{
    
    
    gl_Position =  lightSpaceMatrix * mvp * vec4(aPos, 1.0);
}
