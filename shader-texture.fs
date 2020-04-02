//  A2_29644490
// COMP 371 Assignment 2
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
uniform bool shadowsOn;



uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

//this is based from learnopengl's shadow tutorial and also lab 8
// I also added PCF to make the shadows look more realistic and fix the bias error.
float ShadowCalculation(vec4 fragPosLightSpace)
{

    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;

    projCoords = projCoords * 0.5 + 0.5;

    float closestDepth = texture(shadowMap, projCoords.xy).r;
  
    float currentDepth = projCoords.z;
    
    //bias for shadow acne using PCF
    float bias = 0.003;

    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;
        }
    }
    shadow /= 24.0;

    return shadow;
}

//when shadows were being rendered when no textures were applied, i wanted to change the bias and PCF sample size so it can make the shadows more darker
//which make it more realistic on the grid when no snow texture is applied to the grid.
float shadowCalculationNoTexture(vec4 fragPosLightSpace)
{

    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;

    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(shadowMap, projCoords.xy).r;

    float currentDepth = projCoords.z;
    
    //bias for shadow acne using PCF
    float bias = 0.006;

    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;
        }
    }
    shadow /= 12.0;

    return shadow;
}


//if statements control how to render if there is texture or no texture, shadow or no shadow
//lighting is phong model based from tutorials from learnopengl and lab 8
void main()
{
    vec3 ambient;
    vec3 result;
    float shadow = ShadowCalculation(FragPosLightSpace);
    float shadowNoText = shadowCalculationNoTexture(FragPosLightSpace);
    if(shadowsOn) {
        if(textureOn) {
            float shininess = 64.0f;
            float ambientStrength = 0.01;
            // ambient
          
            ambient =  ambientStrength * texture(textureSampler, vertexUV).rgb;
            
        
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
                    result = (ambient  + diffuse + specular) * color;
           
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
                
                vec3 result =  (ambient  + diffuse + specular) * color  * (1.0 - shadowNoText) ;

                FragColor = vec4(result,1.0f);
                
              
            }
    }else
    {
        if(textureOn) {
                   float shininess = 64.0f;
                   float ambientStrength = 0.01;
                   // ambient
                    vec3 ambient =  ambientStrength * lightColor;
               
                   // diffuse
                   vec3 norm = normalize(Normal);
                   vec3 lightDir = normalize(lightPos - FragPos);
                   float diff = max(dot(norm, lightDir), 0.0);
                   vec3 diffuse = lightColor * diff;
                   
                   // specular
                    float specularStrength = 0.5;
                   vec3 viewDir = normalize(viewPos - FragPos);
                   vec3 reflectDir = reflect(-lightDir, norm);
                   float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
                   vec3 specular = specularStrength * spec * lightColor;
               
                  
              
                       if(withText)
                        {
                           result = (ambient + diffuse + specular) * texture(textureSampler, vertexUV).rgb ;
                  
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
        
    
}


