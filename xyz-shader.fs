//  A1_29644490
// COMP 371 Assignment 1
// Created by Matthew Salaciak 29644490.


#version 330 core


in vec3 vertexColor;


out vec3 color;

uniform vec3 xyz_color;


void main(){
    color = xyz_color;
}
