//  A1_29644490
// COMP 371 Assignment 1
// Created by Matthew Salaciak 29644490.


#version 330 core


in vec3 vertexColor;


out vec4 color;

uniform vec3 olaf_color;


void main(){
    color = vec4(olaf_color.r,olaf_color.g,olaf_color.b,1);
}
