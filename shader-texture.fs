//  A1_29644490
// COMP 371 Assignment 1
// Created by Matthew Salaciak 29644490.



#version 330 core



in vec3 vertexColor;
in vec2 vertexUV;
in vec3 Normal;
in vec3 FragPos;
in vec4 FragPosLightSpace;

out vec4 FragColor;

uniform sampler2D textureSampler;
uniform sampler2D shadowMap;

uniform vec3 color;
uniform  bool textureOn;
uniform bool withText;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;

    return shadow;
}



void main()
{
    vec3 result;
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
        
        float shadow = ShadowCalculation(FragPosLightSpace);
       
        if(withText)
        {
        result = (ambient + (1.0 - shadow) * (diffuse + specular)) ;
           
            
        }else
        {
            float ambientStrength = 0.01;
            vec3 ambient = ambientStrength * lightColor;
            
            //diffuse
            vec3 norm = normalize(Normal);
            vec3 lightDirection = normalize(lightPos - FragPos);
            float diff = max(dot(norm, lightDirection), 0.0f);
            vec3 diffuse = diff * lightColor;
            
            float specularStrength = 0.5;
            vec3 viewDir = normalize(viewPos - FragPos);
            vec3 reflectDir = reflect(-lightDirection, norm);
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
            vec3 specular = specularStrength * spec * lightColor;
            result = (ambient + diffuse + specular) * color;
           
        }

         FragColor = vec4(result,1.0f);
        
    }else
     {
        //ambient
        float ambientStrength = 0.01;
        vec3 ambient = ambientStrength * lightColor;
        
        //diffuse
        vec3 norm = normalize(Normal);
        vec3 lightDirection = normalize(lightPos - FragPos);
        float diff = max(dot(norm, lightDirection), 0.0f);
        vec3 diffuse = diff * lightColor;
        
        float specularStrength = 0.5;
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDirection, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
        vec3 specular = specularStrength * spec * lightColor;
        
        vec3 result = (ambient + diffuse + specular) * color;

        FragColor = vec4(result,1.0f);
        
      
    }
    
}

