//
// COMP 371 Assignment 1
//
// Created by Matthew Salaciak 29644490.
//
// Inspired by the COMP 371 Lectures and Lab 2,3 and 4 and the following tutorials:
// - https://learnopengl.com/Getting-started/Hello-Triangle
// - https://learnopengl.com/Advanced-OpenGL/Advanced-GLSL (for shader class)
// - http://www.opengl-tutorial.org/beginners-tutorials/tutorial-3-matrices/


#include <iostream>
#include <list>
#define GLEW_STATIC 1
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>
#include "vertices.h"
#include "Shader.h"
#include <time.h>
#include "texture-loader.h"


//define namespaces for glm and c++ std
using namespace glm;
using namespace std;

//global variables and functions
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void renderGrid(const GLuint &shader_grid);
void renderOlaf(const GLuint &shader_olaf);
void renderXYZ(const GLuint &XYZ_Shader);
void renderLight(const GLuint &lamp_Shader);
void renderScene(const GLuint &textureShader);

bool textureOn = true;
bool shadowsOn = true;
vec3 lightpos (0.f, 30.0f,0.f);
float legMoving =1;
float armMoving =1;
float gloveMoving =1;

//worldspace matrix

mat4 projectionMatrix = mat4(1.0f);
mat4 viewMatrix;
mat4 modelMatrix = mat4(1.0f);
mat4 modelViewProjection;

//define our identity matrix's for translate, rotate and scale olaf
//this is used later on when I implement the key binding fuctions to control the olafs movements
mat4 translateOlaf(1.f);
mat4 scaleOlaf(1.f);
mat4 rotateOlaf = rotate(mat4(1.0f), radians(90.0f),vec3(00.0f,90.0f, 0.0f));
mat4 legMove (1.f);
mat4 armMove (1.f);
mat4 gloveMove (1.f);
mat4 WorldTransformMatrix(1.f);
//textures
GLuint snowTextureID;
GLuint carrotTextureID;
GLuint shinyMetalTextureID;
GLuint vertexArrayObjects[6], vertexBufferObjects[6];
GLuint depthMap;

//primatative rendering options
int primativeRender = GL_TRIANGLES;
int primativeRender1 = GL_TRIANGLE_STRIP;

//camera info
vec3 cameraPosition(0.0f,15.0f,30.0f);
vec3 cameraLookAt(0.0f, 0.0f, 0.0f);
vec3 cameraUp(0.0f, 1.0f, 0.0f);


    
    

int main(int argc, char*argv[])
{
    
    //random number initialization and int primativeRender is used to store the default rendering option which is GL_TRIANGLES
    srand (time(NULL));

    float  fovAngle = 45.0f;
    numOfVerticesSphere = sizeof(vertexBuffer) / sizeof(Vertex);
    numOfVerticesGrid = sizeof(grid_vertices) / sizeof(grid);
    
    // Initialize GLFW and OpenGL version
   
    glfwInit();
 
#if defined(PLATFORM_OSX)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#else
    // On windows, we set OpenGL version to 2.1, to support more hardware
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
#endif


    
    // Create Window and rendering context using GLFW, resolution is 1024x768
    GLFWwindow* window = glfwCreateWindow(1024, 768, "A1_29644490", NULL, NULL);
    if (window == NULL)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwMakeContextCurrent(window);
//    glViewport(0, 0, 1024, 768);
    glfwSetKeyCallback(window, key_callback);

    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to create GLEW" << std::endl;
        glfwTerminate();
        return -1;
    }
    
            // Load Textures
        #if defined(PLATFORM_OSX)
            snowTextureID = loadTexture("/Users/matthew/Documents/school/WINTER 2020/COMP 371/assignments/A1_29644490/Assignment1_Framework/Xcode/Textures/snowtexture3.jpg");
            carrotTextureID = loadTexture("/Users/matthew/Documents/school/WINTER 2020/COMP 371/assignments/A1_29644490/Assignment1_Framework/Xcode/Textures/carrot.jpg");
            shinyMetalTextureID = loadTexture("/Users/matthew/Documents/school/WINTER 2020/COMP 371/assignments/A1_29644490/Assignment1_Framework/Xcode/Textures/shinymetal.jpg");
        #else
            GLuint brickTextureID = loadTexture("../Assets/Textures/brick.jpg");
            GLuint carrot = loadTexture("../Assets/Textures/cement.jpg");
            GLuint shinymetal = loadTexture("../Assets/Textures/cement.jpg");
        #endif

      
     // the following functions loads the shaders for nontexture, xyz axis and lamp (lightsource)


    
    GLuint XYZ_Shader = Shader("/Users/matthew/Documents/school/WINTER 2020/COMP 371/assignments/A1_29644490/Assignment1_Framework/Source/xyz-shader.vs", "/Users/matthew/Documents/school/WINTER 2020/COMP 371/assignments/A1_29644490/Assignment1_Framework/Source/xyz-shader.fs");
    
    GLuint lamp_Shader = Shader("/Users/matthew/Documents/school/WINTER 2020/COMP 371/assignments/A1_29644490/Assignment1_Framework/Source/lampShader.vs", "/Users/matthew/Documents/school/WINTER 2020/COMP 371/assignments/A1_29644490/Assignment1_Framework/Source/lampShader.fs");
       
    
    //texture shader for grid, olaf
    
       GLuint textureShader = Shader("/Users/matthew/Documents/school/WINTER 2020/COMP 371/assignments/A1_29644490/Assignment1_Framework/Source/shader-texture.vs", "/Users/matthew/Documents/school/WINTER 2020/COMP 371/assignments/A1_29644490/Assignment1_Framework/Source/shader-texture.fs");
    
    
    //shader for simple shadows
    GLuint simpleShadow = Shader("/Users/matthew/Documents/school/WINTER 2020/COMP 371/assignments/A1_29644490/Assignment1_Framework/Source/simple-shadow-shader.vs", "/Users/matthew/Documents/school/WINTER 2020/COMP 371/assignments/A1_29644490/Assignment1_Framework/Source/simple-shadow-shader.fs");

    
        // Define and upload geometry to the GPU here by creating a VAO and VBO that has the size of 3
        // This way we can store of the geometry of all three objects at different indices.
    
        glGenVertexArrays(6, vertexArrayObjects);
        glGenBuffers(6, vertexBufferObjects);
          
          //bind grid vertices
        glBindVertexArray(vertexArrayObjects[0]);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjects[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(grid_vertices), grid_vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(grid), (void*)0);
        glEnableVertexAttribArray(0);
    
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(grid), (void*)sizeof(vec3));
        glEnableVertexAttribArray(1);
    
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(grid), (void*)(2*sizeof(vec3)));
        glEnableVertexAttribArray(2);
    
        //bind xyz vertices
        glBindVertexArray(vertexArrayObjects[1]);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjects[1]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(xyz_verticles), xyz_verticles, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
          
          //bind olaf vertices (use for nose / arms / legs / hat)

            glBindVertexArray(vertexArrayObjects[2]);
            glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjects[2]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(snowman_vertices), snowman_vertices, GL_STATIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(snowman), (void*)0);
            glEnableVertexAttribArray(0);
    
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(snowman), (void*)sizeof(vec3));
            glEnableVertexAttribArray(1);
       
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(snowman), (void*)(2*sizeof(vec3)));
            glEnableVertexAttribArray(2);
    

         //bind olaf sphere for body / upper body / head / eyes / mouth / buttons / gloves
            glBindVertexArray(vertexArrayObjects[3]);
            glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjects[3]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBuffer), vertexBuffer, GL_STATIC_DRAW);
            //sphere vertex position
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
            glEnableVertexAttribArray(0);
            //sphere vertex normal position
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(vec3));
            glEnableVertexAttribArray(1);
        
            //normals for light source
            glBindVertexArray(vertexArrayObjects[4]);
            glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjects[4]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(lightcube), lightcube, GL_STATIC_DRAW);
             //cube vertex position
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(lamp), (void*)0);
            glEnableVertexAttribArray(0);
             //cube vertex normal position
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(lamp), (void*)sizeof(vec3));
            glEnableVertexAttribArray(1);
    
    
       //bind grid vertices notexture
            glBindVertexArray(vertexArrayObjects[5]);
            glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjects[5]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(grid_vertices_noTexture), grid_vertices_noTexture, GL_STATIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(gridNoTexture), (void*)0);
            glEnableVertexAttribArray(0);
    
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(gridNoTexture), (void*)sizeof(vec3));
            glEnableVertexAttribArray(1);
    
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(gridNoTexture), (void*)(2*sizeof(vec3)));
            glEnableVertexAttribArray(2);
    


            // Other OpenGL states to set once
            // Enable Backface culling
            glEnable(GL_CULL_FACE);
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_LESS);


        
            // For frame time
            float lastFrameTime = glfwGetTime();
            double lastMousePosX, lastMousePosY;
            glfwGetCursorPos(window, &lastMousePosX, &lastMousePosY);
        
            //camera information for mouse implementation
            float cameraSpeed = 0.5f;
            float cameraFastSpeed = 2 * cameraSpeed;
            float cameraHorizontalAngle = 90.0f;
            float cameraVerticalAngle = 0.0f;
    
    
    
    //shadow depth map
    const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
        unsigned int depthMapFBO;
        glGenFramebuffers(1, &depthMapFBO);
        // create depth texture

        glGenTextures(1, &depthMap);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
        // attach depth texture as FBO's depth buffer
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    
    
    
    
    //textures
            glUseProgram(textureShader);
            glUniform1i(glGetUniformLocation(textureShader, "textureSampler"), 0);
            glUniform1i(glGetUniformLocation(textureShader, "shadowMap"), 1);
            glUniform1i(glGetUniformLocation(textureShader, "shinyHatSampler"), 2);
    

    

        
        // Entering Main Loop
        while(!glfwWindowShouldClose(window))
        {
      
            float dt = glfwGetTime() - lastFrameTime;
            lastFrameTime += dt;
            bool fastCam = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS;
            float currentCameraSpeed = (fastCam) ? cameraFastSpeed : cameraSpeed;
   

            
             projectionMatrix = perspective(radians(fovAngle),1024.0f / 768.0f, 0.1f,100.0f);
             viewMatrix = lookAt(cameraPosition, cameraPosition + cameraLookAt,cameraUp );
            viewMatrix = viewMatrix * WorldTransformMatrix;

            //setting up the MVP of the world so I can place our objects within
             modelViewProjection = projectionMatrix * viewMatrix * modelMatrix;
 
            // set the background color to the greenish grey
            glClearColor(0.2f, 0.29f, 0.29f,1.0f);
    
            // clear the color and depth buffer at the beginning of each loop
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
          


            //texture shader
            glUseProgram(textureShader);

            //set view and projection materix in shader
            GLuint viewMatrix_texture = glGetUniformLocation(textureShader, "view");
            GLuint projectionMatrix_texture = glGetUniformLocation(textureShader, "projection");
            glUniformMatrix4fv(viewMatrix_texture, 1, GL_FALSE, &viewMatrix[0][0]);
            glUniformMatrix4fv(projectionMatrix_texture, 1, GL_FALSE, &projectionMatrix[0][0]);


            //set light and view position
            GLuint lightPositionTexture = glGetUniformLocation(textureShader, "lightPos");
            GLuint viewPositionTexture = glGetUniformLocation(textureShader, "viewPos");
            glUniform3f(lightPositionTexture, lightpos.x,lightpos.y,lightpos.z);
            glUniform3f(viewPositionTexture, cameraPosition.x,cameraPosition.y,cameraPosition.z);

            //set lightColor for textureShader
            GLuint lightColor = glGetUniformLocation(textureShader, "lightColor");
            glUniform3f(lightColor, 1.0f,1.0f,1.0f);

            
            if (shadowsOn) {
            mat4 lightProjection, lightView , lightSpaceMatrix;

            glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
            glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
            glClear(GL_DEPTH_BUFFER_BIT);


            // 1. render depth of scene to texture (from light's perspective)
              // --------------------------------------------------------------
              float near_plane = 1.0f, far_plane = 100.0f;

              // note that if you use a perspective projection matrix you'll have to change the light position as the current light position isn't enough to reflect the whole scene
              lightProjection = glm::perspective(glm::radians(130.0f), (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT, near_plane, far_plane);
              

              lightView = glm::lookAt(lightpos, glm::vec3(0.0f), glm::vec3(0.0, 0.0, 1.0));
              lightSpaceMatrix = lightProjection * lightView;
              // render scene from light's point of view
              glUseProgram(simpleShadow);
              GLuint lightSpaceMatrixSimple = glGetUniformLocation(simpleShadow, "lightSpaceMatrix");
              glUniformMatrix4fv(lightSpaceMatrixSimple, 1, GL_FALSE, &lightSpaceMatrix[0][0]);

              renderScene(simpleShadow);

              glBindFramebuffer(GL_FRAMEBUFFER, 0);
              // reset viewport
              glViewport(0, 0, 1024, 768);
              glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


              glViewport(0, 0, 1024, 768);
              glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glUseProgram(textureShader);
            glUniform1ui(glGetUniformLocation(textureShader, "shadowsOn"), 1);
            GLuint lightSpaceMatrixShader = glGetUniformLocation(textureShader, "lightSpaceMatrix");
            glUniformMatrix4fv(lightSpaceMatrixShader, 1, GL_FALSE, &lightSpaceMatrix[0][0]);
            }else {
                glUseProgram(textureShader);
                 glUniform1ui(glGetUniformLocation(textureShader, "shadowsOn"), 0);
            }
            

            renderScene(textureShader);
            
            
          
            renderLight(lamp_Shader);
              renderXYZ(XYZ_Shader);
            
            
                      

            // End Frame
            glfwSwapBuffers(window);
            glfwPollEvents();
            
            
            //this part here which controls the camera via the mouse X,Y inputs
            //it is edited and adapted from my solution to lab 4
            double mousePosX, mousePosY;
            glfwGetCursorPos(window, &mousePosX, &mousePosY);
                       
            double dx = mousePosX - lastMousePosX;
            double dy = mousePosY - lastMousePosY;
                       
            lastMousePosX = mousePosX;
            lastMousePosY = mousePosY;
                       
            // Convert to spherical coordinates
            const float cameraAngularSpeed = 10.0f;
            cameraHorizontalAngle -= dx * cameraAngularSpeed * dt;
            cameraVerticalAngle   -= dy * cameraAngularSpeed * dt;
                       
            // Clamp vertical angle to -180,180 (for full rotation!)
            cameraVerticalAngle = std::max(-180.0f, std::min(180.0f, cameraVerticalAngle));
            if (cameraHorizontalAngle > 360)
                {
                    cameraHorizontalAngle -= 360;
                }
            else if (cameraHorizontalAngle < -360)
                {
                    cameraHorizontalAngle += 360;
                }
                       
            float theta = radians(cameraHorizontalAngle);
            float phi = radians(cameraVerticalAngle);
                       
            cameraLookAt = vec3(cosf(phi)*cosf(theta), sinf(phi), -cosf(phi)*sinf(theta));
            vec3 cameraSideVector = cross(cameraLookAt, vec3(0.0f, 1.0f, 0.0f));
            normalize(cameraSideVector);

             
            
            //these are the following keybindings to control the olaf, the camera and the world orientation, textures, lighting and shadows
            
            if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS) // camera zoom in
                          {
                              cameraPosition.z -= currentCameraSpeed * dt*40;
                              
                          }
                                            
                      if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS) // camera zoom out
                          {
                              cameraPosition.z += currentCameraSpeed * dt*40;
                          }
            
            
            
            if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS ) // move camera to the left
                {
                    cameraPosition.x -= currentCameraSpeed * dt*40;
                    
                }

            if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) // move camera to the right
                {
                    cameraPosition.x += currentCameraSpeed * dt*40;
                }
            
            if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) // camera zoom in
                {
                    fovAngle = fovAngle  - 0.1f;
                }
                                  
            if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) // camera zoom out
                {
                    fovAngle = fovAngle + 0.1f;
                }

            if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS) // move camera down
                {
                    cameraPosition.y -= currentCameraSpeed * dt*40;
                }

            if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS) // move camera up
                {
                    cameraPosition.y += currentCameraSpeed * dt*40;
                }
           
         
            
            if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS ) // move olaf to the left
                {
                    float xTrans = -0.1;
                    xTrans = xTrans + 0.001f;
                    
                    legMoving +=0.1;
                    armMoving -=0.1;
                    gloveMoving +=0.1;
                    
                    float theta = cos(legMoving);
                    theta = theta*8;
                    float thetaArm = cos(armMoving);
                    thetaArm = thetaArm*4;
                    float thetaGlove = cos(gloveMoving);
thetaGlove = thetaGlove*.08;
                    legMove = rotate(mat4(1.0f), radians(theta),vec3(0.0f,0.0f, -0.1f));
                    armMove = rotate(mat4(1.0f), radians(thetaArm),vec3(1.0f,0.0f, 0.0f));
                    translateOlaf = translateOlaf * translate(mat4(1.0f), vec3(xTrans, 0.0f, 0.0f));
                    gloveMove = translate(mat4(1.0f), vec3(0.0f, thetaGlove, 0.0f));
                }
              
            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) // move olaf to the right
                {
                    float xTrans = 0.1;
                    xTrans = xTrans + 0.001f;
                    
                    legMoving -=0.1;
                    armMoving -=0.1;
                     gloveMoving +=0.1;

                    float theta = cos(legMoving);
                    theta = theta*8;
                    float thetaArm = cos(armMoving);
                    thetaArm = thetaArm*4;
                    float thetaGlove = cos(gloveMoving);
thetaGlove = thetaGlove*.08;
                    armMove = rotate(mat4(1.0f), radians(thetaArm),vec3(1.0f,0.0f, 0.0f));
                    legMove = rotate(mat4(1.0f), radians(theta),vec3(0.0f,0.0f, -0.1f));
                    translateOlaf = translateOlaf * translate(mat4(1.0f), vec3(xTrans,0.0f, 0.0f));
                    gloveMove = translate(mat4(1.0f), vec3(0.0f, thetaGlove, 0.0f));
                }
              
            if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) // move olaf up
                {
                    float zTrans = 0.1;
                    zTrans = zTrans + 0.001f;
              
                    legMoving +=0.1;
                    armMoving -=0.1;
                     gloveMoving +=0.1;
                    
                    float theta = cos(legMoving);
                    theta = theta*8;
                    float thetaArm = cos(armMoving);
                    thetaArm = thetaArm*4;
                    float thetaGlove = cos(gloveMoving);
thetaGlove = thetaGlove*.08;
                    armMove = rotate(mat4(1.0f), radians(thetaArm),vec3(1.0f,0.0f, 0.0f));
                    legMove = rotate(mat4(1.0f), radians(theta),vec3(0.0f,0.0f, -0.1f));
                    translateOlaf = translateOlaf * translate(mat4(1.0f), vec3(0.0f,0.0f, zTrans));
                    gloveMove = translate(mat4(1.0f), vec3(0.0f, thetaGlove, 0.0f));
                }
              
            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) // move olaf down
                {
                    float zTrans = -0.1;
                    zTrans = zTrans + 0.001f;
                    
                    legMoving +=0.1;
                    armMoving -=0.1;
                     gloveMoving +=0.1;
                    
                    float theta = cos(legMoving);
                    theta = theta*8;
                    float thetaArm = cos(armMoving);
                    thetaArm = thetaArm*4;
                    float thetaGlove = cos(gloveMoving);
                    thetaGlove = thetaGlove*.08;
                                                
                    armMove = rotate(mat4(1.0f), radians(thetaArm),vec3(1.0f,0.0f, 0.0f));
                    legMove = rotate(mat4(1.0f), radians(theta),vec3(0.0f,0.0f, -0.1f));
                    translateOlaf = translateOlaf * translate(mat4(1.0f), vec3(0.0f,0.0f, zTrans));
                    gloveMove = translate(mat4(1.0f), vec3(0.0f, thetaGlove, 0.0f));
                 

                }
            
            
            if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) // random position
                 {
                     float xTrans = rand() % 7 + (-3);
                     float  zTrans = rand() % 7 + (-3);
                     translateOlaf = translateOlaf * translate(mat4(1.0f), vec3(xTrans, 0.0f, zTrans));
                 }
            
            if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) // scale up
                {
                    float x =1;
                    float y=1;
                    float z=1;
                    x+=0.01;
                    y+=0.01;
                    z+=0.01;
                    scaleOlaf = scaleOlaf  * scale(mat4(1.0f), vec3(x, y, z));
                }

            
            if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) // scale down
                {
                                       
                    float x =1;
                    float y=1;
                    float z=1;
                    x-=0.01;
                    y-=0.01;
                    z-=0.01;
                    scaleOlaf = scaleOlaf  * scale(mat4(1.0f), vec3(x, y, z));
                                     
                }
            
            
            if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) // primative change to trianges
                {
                    primativeRender = GL_TRIANGLES;
                    primativeRender1 = GL_TRIANGLE_STRIP;
                }
            
            if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) // primative change to points
                {
                    primativeRender = GL_POINTS;
                    primativeRender1 = GL_POINTS;
                }
            
            if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) // primative change to lines
                {
                primativeRender = GL_LINES;
                primativeRender1 = GL_LINES;
                }
      
            
            
              if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) // rotate around the y axis counter clockwise
                {
                                                   
                    float x =5.01;
                    float y=5.01;
                    float z=5.01;
                    x-=0.01;
                    y-=0.01;
                    z-=0.01;
                    rotateOlaf = rotateOlaf * rotate(mat4(1.0f), radians(x),vec3(0.0f,1.f, 0.f));
                }
            
            if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) // rotate around the y axis clockwise
                {
                                                            
                             float x =5.01;
                             float y=5.01;
                             float z=5.01;
                             x+=0.01;
                             y+=0.01;
                             z+=0.01;
                             rotateOlaf = rotateOlaf * rotate(mat4(1.0f), radians(x),vec3(0.0f,-1.f, 0.f));
                         }
            
            
           
            
            if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) // rotate x and z together counter clockwise
                                   {
                                                                      
                                       float x =5.01;
                                       float y=5.01;
                                       float z=5.01;
                                       x+=0.01;
                                       y+=0.01;
                                       z+=0.01;
                                       rotateOlaf = rotateOlaf * rotate(mat4(1.0f), radians(x),vec3(-1.0f,0.0f, -1.f));
                                   }
            
            if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) // rotate x and z together clockwise
            {
                                               
                float x =5.01;
                float y=5.01;
                float z=5.01;
                x+=0.01;
                y+=0.01;
                z+=0.01;
                rotateOlaf = rotateOlaf * rotate(mat4(1.0f), radians(x),vec3(1.0f,0.f, 1.f));
            }
            
            
            if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) // rotate X axis in the other orientation of the world
                     {
                                                        
                float x =5.01;
                float y=5.01;
                float z=5.01;
                x+=0.01;
                y+=0.01;
                z+=0.01;
                       
                         WorldTransformMatrix = WorldTransformMatrix * rotate(mat4(1.0f), radians(x),vec3(-1.0f,0.f, 0.f));

                               
                     }
            
            if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) // rotate X axis in the other orientation of the world
                             {
                                                                
                float x =5.01;
                float y=5.01;
                float z=5.01;
                x+=0.01;
                y+=0.01;
                z+=0.01;
                 WorldTransformMatrix = WorldTransformMatrix * rotate(mat4(1.0f), radians(x),vec3(-1.0f,0.f, 0.f));
                             
            

                                     
                             }
                                        
            
            
            
            if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) //rotate Y axis of the world
                             {
                                                                
                float x =5.01;
                float y=5.01;
                float z=5.01;
                x+=0.01;
                y+=0.01;
                z+=0.01;
                WorldTransformMatrix = WorldTransformMatrix * rotate(mat4(1.0f), radians(x),vec3(0.0f,1.f, 0.f));
                             }
            
            
            if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) // rotate Y axis of the world in the other orientation
                             {
                                                                
                float x =5.01;
                float y=5.01;
                float z=5.01;
                x+=0.01;
                y+=0.01;
                z+=0.01;
                WorldTransformMatrix = WorldTransformMatrix * rotate(mat4(1.0f), radians(x),vec3(0.0f,-1.0f, 0.f));
                             }
            
            if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS) // reset world orientation to original settings
                            {
                                                                         
           
              WorldTransformMatrix = mat4(1.0f);
                                      }
            
            if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
                 glfwSetWindowShouldClose(window, true);
            
            
            
            viewMatrix = lookAt(cameraPosition, cameraPosition + cameraLookAt, cameraUp );
            projectionMatrix = perspective(radians(fovAngle),1024.0f / 768.0f, 0.1f,100.0f);
            
            
         
        }

        
        // Shutdown GLFW
        glfwTerminate();
        
        return 0;
    }

 
    
     void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
     {
         if (key == GLFW_KEY_X && action == GLFW_PRESS)
             textureOn = !textureOn;
         
         if (key == GLFW_KEY_B && action == GLFW_PRESS)
                     shadowsOn = !shadowsOn;
         
         if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) // turn left
                                   
             rotateOlaf = rotateOlaf * rotate(mat4(1.0f), radians(90.0f),vec3(0.0f,-1.f, 0.f));
                                            
         if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) // turn right
                                          
             rotateOlaf = rotateOlaf * rotate(mat4(1.0f), radians(-90.0f),vec3(0.0f,-1.f, 0.f));
         
         if (key == GLFW_KEY_W && action == GLFW_RELEASE)
         {  legMove =mat4(1.0f);
             armMove = mat4(1.0f);
             gloveMove = mat4(1.0f);
         }
         
         if (key == GLFW_KEY_A && action == GLFW_RELEASE)
        {  legMove =mat4(1.0f);
            armMove = mat4(1.0f);
            gloveMove = mat4(1.0f);
        }
         
         if (key == GLFW_KEY_S && action == GLFW_RELEASE)
         {  legMove =mat4(1.0f);
             armMove = mat4(1.0f);
             gloveMove = mat4(1.0f);
         }
         
         if (key == GLFW_KEY_D && action == GLFW_RELEASE)
         {  legMove =mat4(1.0f);
             armMove = mat4(1.0f);
             gloveMove = mat4(1.0f);
         }
                                            
         
     }
   
void renderScene(const GLuint &textureShader)
{
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, snowTextureID);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, depthMap);
            renderGrid(textureShader);
    
    
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, depthMap);
            renderOlaf(textureShader);
        

}




    
void renderXYZ(const GLuint &XYZ_Shader)

{
    glEnableVertexAttribArray(0);
    glBindVertexArray(vertexArrayObjects[1]);
    glUseProgram(XYZ_Shader);
                                        
//get the MVP of the XYZ from the shader, get the color uniform variable so we can set each line to its own color
    GLuint modelViewProjection_XYZ = glGetUniformLocation(XYZ_Shader, "mvp");
    GLuint XYZ_color = glGetUniformLocation(XYZ_Shader, "xyz_color");
    //create the MVP of the camera to be placed within the world
    mat4 camera_ModelViewProject_XYZ =  projectionMatrix * viewMatrix * modelMatrix;
    glUniformMatrix4fv(modelViewProjection_XYZ, 1, GL_FALSE, &camera_ModelViewProject_XYZ[0][0]);
//quick for loop to draw the 3 lines, and takes its colors from the XYZ colors array which is stored in the vertices header
    int XYZ_vertexIndex =0;
    for(int i =0; i<3;i++)
    {
        glUniform3f(XYZ_color, XYZ_Colors[i].x,XYZ_Colors[i].y,XYZ_Colors[i].z);
        glDrawArrays(GL_LINES, XYZ_vertexIndex, 2);
        XYZ_vertexIndex = XYZ_vertexIndex +2;
      }
//
}

void renderLight(const GLuint &lamp_Shader)
{
    //lamp (light source)
    glEnableVertexAttribArray(0);
    glBindVertexArray(vertexArrayObjects[4]);
    glUseProgram(lamp_Shader);
                        
     mat4 WorldView_lamp = projectionMatrix * viewMatrix * modelMatrix * translate(mat4(1.0f),lightpos) * scale(mat4(1.0f),vec3(3.0f,3.0f,3.0f));
     GLuint lampshader_MVP = glGetUniformLocation(lamp_Shader, "mvp");
     glUniformMatrix4fv(lampshader_MVP, 1, GL_FALSE, &WorldView_lamp[0][0]);
     glDrawArrays(primativeRender, 0, 36);
      glBindVertexArray(0);
}

    
    void renderGrid(const GLuint &shader_grid)
{
    

             //get the MVP of the grid from the shader
               
               GLuint modelViewProjection_XZ_GRID_Texture = glGetUniformLocation(shader_grid, "mvp");
               GLuint gridcolor = glGetUniformLocation(shader_grid, "color");
          

               //no texture
               if (!textureOn) {
               glEnableVertexAttribArray(0);
               glBindVertexArray(vertexArrayObjects[5]);
               glUniform1ui(glGetUniformLocation(shader_grid, "textureOn"), 0);
               

               // Draw grid
               for(int j=-50; j<=50; ++j)
               {
                   for(int i=-50; i<=50; ++i)
                       {
                     mat4 grid = translate(mat4(1.0f), vec3(i, 0.f, j));
                     glUniformMatrix4fv(modelViewProjection_XZ_GRID_Texture, 1, GL_FALSE, &grid[0][0]);
                     glUniform3f(gridcolor, 0.56f, 0.45f, 0.13f);
                     glDrawArrays(GL_LINE_LOOP, 0, numOfVerticesGrid);
                       }
               }
                   }
               
               //texture
               if (textureOn) {
               glUniform1ui(glGetUniformLocation(shader_grid, "withText"), 1);
               glUniform1i(glGetUniformLocation(shader_grid, "textureOn"), 1);
               glEnableVertexAttribArray(0);
               glBindVertexArray(vertexArrayObjects[0]);
              



               // Draw grid
               for(int j=-50; j<=50; ++j)
                   {
                   for(int i=-50; i<=50; ++i)
                       {
                           mat4 grid = translate(mat4(1.0f), vec3(i, 0.f, j));
                           glUniformMatrix4fv(modelViewProjection_XZ_GRID_Texture, 1, GL_FALSE, &grid[0][0]);
                           glDrawArrays(GL_TRIANGLES, 0, numOfVerticesGrid);
                       }
                   }
               }
      glBindVertexArray(0);
    
}

    
    void renderOlaf(const GLuint &shader_olaf)

{
    //            //geometry for the olaf
    
                glEnableVertexAttribArray(0);
                glBindVertexArray(vertexArrayObjects[3]);
    
                glActiveTexture(GL_TEXTURE0);
//
                glUniform1ui(glGetUniformLocation(shader_olaf, "withText"), 0);
                glFrontFace(GL_CW);
    
                //get the worldview of the olaf within the scene
                mat4 WorldView_Olaf =  translateOlaf  * rotateOlaf * scaleOlaf;
    
                //get the mvp of the olaf and the olaf uniform color variable so we can use it to make eyes, buttons, gloves, nose, scarf.
    
                GLuint modelViewProjection_Olaf = glGetUniformLocation(shader_olaf, "mvp");
                GLuint olaf_Color = glGetUniformLocation(shader_olaf, "color");
    
                //body
                mat4 olaf_Body = WorldView_Olaf * translate(mat4(1.0f), vec3(0.f, 3.5f, 0.f))* scale(mat4(1.0f), vec3(1.3f, 1.3f, 1.3f));
                glUniformMatrix4fv(modelViewProjection_Olaf, 1, GL_FALSE, &olaf_Body[0][0]);
                glUniform3f(olaf_Color, 1.0f,1.0f,1.0f);
                glDrawArrays(primativeRender1, 0, numOfVerticesSphere);
    
                //upper body
                olaf_Body = WorldView_Olaf * translate(mat4(1.0f), vec3(0.f, 4.65f, 0.f)) * scale(mat4(1.0f), vec3(0.95f, 0.95f, 0.95f));
                glUniformMatrix4fv(modelViewProjection_Olaf, 1, GL_FALSE, &olaf_Body[0][0]);
                glUniform3f(olaf_Color, 1.0f,1.0f,1.0f);
                glDrawArrays(primativeRender1, 0, numOfVerticesSphere);
    
                //head
                olaf_Body = WorldView_Olaf * translate(mat4(1.0f), vec3(0.f, 5.95f, 0.f)) * scale(mat4(1.0f), vec3(0.7f, 0.7f, 0.7f));
                glUniformMatrix4fv(modelViewProjection_Olaf, 1, GL_FALSE, &olaf_Body[0][0]);
                glUniform3f(olaf_Color, 1.0f,1.0f,1.0f);
                glDrawArrays(primativeRender1, 0, numOfVerticesSphere);
     
                glBindVertexArray(0);
                //switch back to cube vertices
                glEnableVertexAttribArray(0);
                glBindVertexArray(vertexArrayObjects[2]);
               
    
                //right leg
                olaf_Body = WorldView_Olaf * translate(mat4(1.0f), vec3(-0.0f, 1.3f, -0.5f)) * inverse(legMove) *scale(mat4(1.0f), vec3(1.0f, 2.5f, -0.4f));
                glUniformMatrix4fv(modelViewProjection_Olaf, 1, GL_FALSE, &olaf_Body[0][0]);
                glUniform3f(olaf_Color, 1.0f,1.0f,1.0f);
                glDrawArrays(primativeRender, 0, 36);
    
                //left leg
                olaf_Body = WorldView_Olaf * translate(mat4(1.0f), vec3(-0.0f, 1.3f, 0.5f)) * legMove * scale(mat4(1.0f), vec3(1.0f, 2.5f, -0.4f));
                glUniformMatrix4fv(modelViewProjection_Olaf, 1, GL_FALSE, &olaf_Body[0][0]);
                glUniform3f(olaf_Color, 1.0f,1.0f,1.0f);
                glDrawArrays(primativeRender, 0, 36);
    
    
                //left arm
                olaf_Body = WorldView_Olaf * translate(mat4(1.0f), vec3(-0.0f, 4.7f, 1.50f)) * inverse(armMove) * rotate(mat4(1.0f), radians(90.0f), vec3(6.0f, 0.0f, 0.0f)) * scale(mat4(1.0f), vec3(0.2f, 1.8f, -0.2f));
                glUniformMatrix4fv(modelViewProjection_Olaf, 1, GL_FALSE, &olaf_Body[0][0]);
               glUniform3f(olaf_Color, 0.39f,0.26f,0.13f);
                glDrawArrays(primativeRender, 0, 36);
    
                //right arm
                olaf_Body = WorldView_Olaf * translate(mat4(1.0f), vec3(-0.0f, 4.7f, -1.50f)) * armMove * rotate(mat4(1.0f), radians(90.0f), vec3(6.0f, 0.0f, 0.0f)) * scale(mat4(1.0f), vec3(0.2f, 1.8f, -0.2f));
                glUniformMatrix4fv(modelViewProjection_Olaf, 1, GL_FALSE, &olaf_Body[0][0]);
               glUniform3f(olaf_Color, 0.39f,0.26f,0.13f);
                glDrawArrays(primativeRender, 0, 36);
    
                glFrontFace(GL_CCW);
                 glUniform1ui(glGetUniformLocation(shader_olaf, "withText"), 1);
    
    
                //nose
                if (!textureOn) {
    
                olaf_Body = WorldView_Olaf * translate(mat4(1.0f), vec3(-0.896f, 5.9f, 0.0f)) * rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 0.0f, 6.0f)) * scale(mat4(1.0f), vec3(-0.09f, 0.35f, -0.1f));
                glUniformMatrix4fv(modelViewProjection_Olaf, 1, GL_FALSE, &olaf_Body[0][0]);
                glUniform3f(olaf_Color, 1.0f,0.64f,0.0f);
                glDrawArrays(primativeRender, 0, 36);
                }
    
    
                //nose textured
    
                if(textureOn) {
                olaf_Body = WorldView_Olaf * translate(mat4(1.0f), vec3(-0.896f, 5.9f, 0.0f)) * rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 0.0f, 6.0f)) * scale(mat4(1.0f), vec3(-0.09f, 0.35f, -0.1f));
                glUniformMatrix4fv(modelViewProjection_Olaf, 1, GL_FALSE, &olaf_Body[0][0]);
                glBindTexture(GL_TEXTURE_2D, carrotTextureID);
                glDrawArrays(primativeRender, 0, 36);
                }
    
                glBindVertexArray(0);
                glEnableVertexAttribArray(0);
                glBindVertexArray(vertexArrayObjects[3]);
                glUniform1ui(glGetUniformLocation(shader_olaf, "withText"), 0);
    
                 glFrontFace(GL_CW);
                //left eye
    
                olaf_Body = WorldView_Olaf * translate(mat4(1.0f), vec3(-0.696f, 6.05f, -0.2f)) * rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 0.0f, 6.0f)) * scale(mat4(1.0f), vec3(-0.09f, 0.01f, -0.1f));
                glUniformMatrix4fv(modelViewProjection_Olaf, 1, GL_FALSE, &olaf_Body[0][0]);
                glUniform3f(olaf_Color, 0.0f,0.0f,0.0f);
                glDrawArrays(primativeRender1, 0, numOfVerticesSphere);
    
                //right eye
                olaf_Body = WorldView_Olaf * translate(mat4(1.0f), vec3(-0.696f, 6.05f, 0.2f)) * rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 0.0f, 6.0f)) * scale(mat4(1.0f), vec3(-0.09f, 0.01f, -0.1f));
                glUniformMatrix4fv(modelViewProjection_Olaf, 1, GL_FALSE, &olaf_Body[0][0]);
                glUniform3f(olaf_Color, 0.0f,0.0f,0.0f);
                glDrawArrays(primativeRender1, 0, numOfVerticesSphere);
    
                //mouth
                olaf_Body = WorldView_Olaf * translate(mat4(1.0f), vec3(-0.698f, 5.7f, 0.0f)) * rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 0.0f, 6.0f)) * scale(mat4(1.0f), vec3(0.1f, 0.01f, 0.2f));
                glUniformMatrix4fv(modelViewProjection_Olaf, 1, GL_FALSE, &olaf_Body[0][0]);
                glUniform3f(olaf_Color, 0.0f,0.0f,0.0f);
                glDrawArrays(primativeRender1, 0, numOfVerticesSphere);
    
                //buttons
                olaf_Body = WorldView_Olaf * translate(mat4(1.0f), vec3(-0.85f, 5.2f, 0.0f)) * rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 0.0f, 6.0f)) * scale(mat4(1.0f), vec3(-0.1f, 0.03f, -0.1f));
                glUniformMatrix4fv(modelViewProjection_Olaf, 1, GL_FALSE, &olaf_Body[0][0]);
                glUniform3f(olaf_Color, 0.39f,0.26f,0.13f);
                glDrawArrays(primativeRender1, 0, numOfVerticesSphere);
    
                //buttons
                olaf_Body = WorldView_Olaf * translate(mat4(1.0f), vec3(-0.94f, 4.9f, 0.0f)) * rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 0.0f, 6.0f)) * scale(mat4(1.0f), vec3(-0.1f, 0.03f, -0.1f));
                glUniformMatrix4fv(modelViewProjection_Olaf, 1, GL_FALSE, &olaf_Body[0][0]);
                glUniform3f(olaf_Color, 0.39f,0.26f,0.13f);
                glDrawArrays(primativeRender1, 0, numOfVerticesSphere);
    
    
                //buttons
                olaf_Body = WorldView_Olaf * translate(mat4(1.0f), vec3(-0.98f, 4.6f, 0.0f)) * rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 0.0f, 6.0f)) * scale(mat4(1.0f), vec3(-0.1f, 0.03f, -0.1f));
                glUniformMatrix4fv(modelViewProjection_Olaf, 1, GL_FALSE, &olaf_Body[0][0]);
                glUniform3f(olaf_Color, 0.39f,0.26f,0.13f);
                glDrawArrays(primativeRender1, 0, numOfVerticesSphere);
    

    
    
                //left glove
                olaf_Body = WorldView_Olaf * translate(mat4(1.0f), vec3(0.0f, 4.7f, -2.6f)) * inverse(gloveMove) * rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 0.0f, 6.0f)) * scale(mat4(1.0f), vec3(-0.3f, -0.3f, -0.3f));
                glUniformMatrix4fv(modelViewProjection_Olaf, 1, GL_FALSE, &olaf_Body[0][0]);
                   glUniform3f(olaf_Color, 1.0f,0.0f,0.0f);
                glDrawArrays(primativeRender1, 0, numOfVerticesSphere);
    
    
                //right glove
                olaf_Body = WorldView_Olaf * translate(mat4(1.0f), vec3(0.0f, 4.7f, 2.6f)) * inverse(gloveMove) * rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 0.0f, 6.0f)) * scale(mat4(1.0f), vec3(-0.3f, -0.3f, -0.3f));
                glUniformMatrix4fv(modelViewProjection_Olaf, 1, GL_FALSE, &olaf_Body[0][0]);
                   glUniform3f(olaf_Color, 1.0f,0.0f,0.0f);
                glDrawArrays(primativeRender1, 0, numOfVerticesSphere);
    
    
    
                //scarf
                olaf_Body = WorldView_Olaf * translate(mat4(1.0f), vec3(0.0f, 5.45f, 0.0f)) * rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 0.0f, 6.0f)) * scale(mat4(1.0f), vec3(0.15f, 1.0f, 0.8f));
                glUniformMatrix4fv(modelViewProjection_Olaf, 1, GL_FALSE, &olaf_Body[0][0]);
                glUniform3f(olaf_Color, 1.0f,0.0f,0.0f);
                glDrawArrays(primativeRender1, 0, numOfVerticesSphere);
    
    
    
                glBindVertexArray(0);
                glEnableVertexAttribArray(0);
                glBindVertexArray(vertexArrayObjects[2]);
    
    
                glFrontFace(GL_CCW);
                glUniform1ui(glGetUniformLocation(shader_olaf, "withText"), 1);
                //hat brim
    
                if(!textureOn) {
                olaf_Body = WorldView_Olaf * translate(mat4(1.0f), vec3(-0.0f, 6.50f, 0.f)) * rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 0.0f, 6.0f)) * scale(mat4(1.0f), vec3(0.1f, 1.2f, 1.75f));
                glUniformMatrix4fv(modelViewProjection_Olaf, 1, GL_FALSE, &olaf_Body[0][0]);
                glUniform3f(olaf_Color, 0.0f,0.0f,0.0f);
                glDrawArrays(primativeRender, 0, 36);
    
                //hat top
                olaf_Body = WorldView_Olaf * translate(mat4(1.0f), vec3(-0.0f, 6.8f, 0.f)) * rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 0.0f, 6.0f)) * scale(mat4(1.0f), vec3(0.7f, 1.2f, 1.4f));
                glUniformMatrix4fv(modelViewProjection_Olaf, 1, GL_FALSE, &olaf_Body[0][0]);
                glUniform3f(olaf_Color, 0.0f,0.0f,0.0f);
                glDrawArrays(primativeRender, 0, 36);
                }
    
                //texture hat brim
 
                if (textureOn) {
    
    
         

            
                olaf_Body = WorldView_Olaf * translate(mat4(1.0f), vec3(-0.0f, 6.50f, 0.f)) * rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 0.0f, 6.0f)) * scale(mat4(1.0f), vec3(0.1f, 1.2f, 1.75f));
                glUniformMatrix4fv(modelViewProjection_Olaf, 1, GL_FALSE, &olaf_Body[0][0]);
                glBindTexture(GL_TEXTURE_2D, shinyMetalTextureID);
                glDrawArrays(primativeRender, 0, 36);
    
                //textured hat top
                olaf_Body = WorldView_Olaf * translate(mat4(1.0f), vec3(-0.0f, 6.8f, 0.f)) * rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 0.0f, 6.0f)) * scale(mat4(1.0f), vec3(0.7f, 1.2f, 1.4f));
                glUniformMatrix4fv(modelViewProjection_Olaf, 1, GL_FALSE, &olaf_Body[0][0]);
       glBindTexture(GL_TEXTURE_2D, shinyMetalTextureID);
                glDrawArrays(primativeRender, 0, 36);
    
                }
                  glUniform1ui(glGetUniformLocation(shader_olaf, "withText"), 0);
                    glBindVertexArray(0);
              
 
    
}
