//  A1_29644490
// COMP 371 Assignment 1
// Created by Matthew Salaciak 29644490.


#version 330 core


in vec3 vertexColor;
in vec3 Normal;
in vec3 FragPos;


out vec4 color;

uniform vec3 olaf_color;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;


void main(){
    
    //ambient
    float ambientStrength = 0.3;
    vec3 ambient = ambientStrength * lightColor;
    
    //diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDirection = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDirection), 0.0);
    vec3 diffuse = diff * lightColor;
    
    float specularStrength = 0.9;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDirection, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;
    
    vec3 result = (ambient + diffuse + specular) * olaf_color;
    color = vec4(result,1);
}
