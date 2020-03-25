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
uniform vec3 viewPos;




void main()
{
    
    if(textureOn) {
        float shininess = 64.0f;
        float ambientStrength = 0.01;

       
     // ambient
        vec3 ambient =  ambientStrength * texture(textureSampler, vertexUV).rgb;
        
        // diffuse
        vec3 norm = normalize(Normal);
        vec3 lightDir = normalize(lightPos - FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = lightColor * diff * texture(textureSampler, vertexUV).rgb;
        
        // specular
         float specularStrength = 0.5;
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
        vec3 specular = specularStrength * spec * lightColor;
            
        vec3 result = ambient + diffuse + specular;
        FragColor = vec4(result, 1.0);
        
   
        
    }else
    if(!textureOn) {
        //ambient
        float ambientStrength = 0.01;
        vec3 ambient = ambientStrength * lightColor;
        
        //diffuse
        vec3 norm = normalize(Normal);
        vec3 lightDirection = normalize(lightPos - FragPos);
        float diff = max(dot(norm, lightDirection), 0.0f);
        vec3 diffuse = diff * lightColor;
        
        float specularStrength = 0.8;
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDirection, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
        vec3 specular = specularStrength * spec * lightColor;
        
        vec3 result = (ambient + diffuse + specular) * color;

        FragColor = vec4(result,1.0f);
        
        
        
      
    }
    
}

