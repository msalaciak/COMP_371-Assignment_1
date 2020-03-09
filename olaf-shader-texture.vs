//  A1_29644490
// COMP 371 Assignment 1
// Created by Matthew Salaciak 29644490.




#version 330 core

        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec2 aUV;
        
       uniform mat4 mvp;
       uniform mat4 model;
       uniform mat4 view;
       uniform mat4 projection;
       in vec3 color;
       out vec3 vertexColor;
       out vec2 vertexUV;


       void main()
       {
           gl_Position = mvp * vec4(aPos, 1.0f);
           vertexColor = color;
           vertexUV = aUV;
           
       }
        
