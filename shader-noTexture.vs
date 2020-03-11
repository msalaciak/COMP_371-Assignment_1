//  A1_29644490
// COMP 371 Assignment 1
// Created by Matthew Salaciak 29644490.




#version 330 core

        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec3 aNormal;
        
       uniform mat4 mvp;
       uniform mat4 model;
       uniform mat4 view;
       uniform mat4 projection;
       in vec3 color;
       out vec3 vertexColor;
       out vec3 Normal;
        out vec3 FragPos;

       void main()
       {
           gl_Position = mvp * vec4(aPos, 1.0f);
           vertexColor = color;
           FragPos = vec3(mvp * vec4(aPos, 1.0f));
           
           Normal = mat3(transpose(inverse(model))) * aNormal;
       }
