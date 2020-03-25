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
bool textureOn = true;
vec3 lightpos (0.f, 30.0f,0.f);






int main(int argc, char*argv[])
{
    
    //define our identity matrix's for translate, rotate and scale olaf, this is used later on when I implement the key binding fuctions to control the olafs movements
    mat4 translateOlaf(1.f);
    mat4 scaleOlaf(1.f);
    mat4 rotateOlaf = rotate(mat4(1.0f), radians(90.0f),vec3(00.0f,90.0f, 0.0f));
    
    
   
    
    //random number initialization and int primativeRender is used to store the default rendering option which is GL_TRIANGLES
    srand (time(NULL));
    int primativeRender = GL_TRIANGLES;
    int primativeRender1 = GL_TRIANGLE_STRIP;
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
    glViewport(0, 0, 1024, 768);
    glfwSetKeyCallback(window, key_callback);

    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to create GLEW" << std::endl;
        glfwTerminate();
        return -1;
    }
    
            // Load Textures
        #if defined(PLATFORM_OSX)
            GLuint snowTextureID = loadTexture("/Users/matthew/Documents/school/WINTER 2020/COMP 371/assignments/A1_29644490/Assignment1_Framework/Xcode/Textures/snowtexture3.jpg");
            GLuint carrotTextureID = loadTexture("/Users/matthew/Documents/school/WINTER 2020/COMP 371/assignments/A1_29644490/Assignment1_Framework/Xcode/Textures/carrot.jpg");
            GLuint shinyMetalTextureID = loadTexture("/Users/matthew/Documents/school/WINTER 2020/COMP 371/assignments/A1_29644490/Assignment1_Framework/Xcode/Textures/shinymetal.jpg");
        #else
            GLuint brickTextureID = loadTexture("../Assets/Textures/brick.jpg");
            GLuint carrot = loadTexture("../Assets/Textures/cement.jpg");
            GLuint shinymetal = loadTexture("../Assets/Textures/cement.jpg");
        #endif

      
     // the following functions loads the shader files and creates my shader objects for the XZ grid, the XYZ coordinate lines ans the olaf

    GLuint nonTexturedShader = Shader("/Users/matthew/Documents/school/WINTER 2020/COMP 371/assignments/A1_29644490/Assignment1_Framework/Source/shader-noTexture.vs", "/Users/matthew/Documents/school/WINTER 2020/COMP 371/assignments/A1_29644490/Assignment1_Framework/Source/shader-noTexture.fs");
    
    GLuint XYZ_Shader = Shader("/Users/matthew/Documents/school/WINTER 2020/COMP 371/assignments/A1_29644490/Assignment1_Framework/Source/xyz-shader.vs", "/Users/matthew/Documents/school/WINTER 2020/COMP 371/assignments/A1_29644490/Assignment1_Framework/Source/xyz-shader.fs");
    
    GLuint lamp_Shader = Shader("/Users/matthew/Documents/school/WINTER 2020/COMP 371/assignments/A1_29644490/Assignment1_Framework/Source/lampShader.vs", "/Users/matthew/Documents/school/WINTER 2020/COMP 371/assignments/A1_29644490/Assignment1_Framework/Source/lampShader.fs");
       
    
    //texture shader
    
       GLuint textureShader = Shader("/Users/matthew/Documents/school/WINTER 2020/COMP 371/assignments/A1_29644490/Assignment1_Framework/Source/shader-texture.vs", "/Users/matthew/Documents/school/WINTER 2020/COMP 371/assignments/A1_29644490/Assignment1_Framework/Source/shader-texture.fs");
    

    
      

      

    
        // Define and upload geometry to the GPU here by creating a VAO and VBO that has the size of 3
        // This way we can store of the geometry of all three objects at different indices.
    
          GLuint vertexArrayObjects[6], vertexBufferObjects[6];
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
//
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
     



    
    // Camera parameters for view transform
         vec3 cameraPosition(0.0f,15.0f,30.0f);
         vec3 cameraLookAt(0.0f, 0.0f, 0.0f);
         vec3 cameraUp(0.0f, 1.0f, 0.0f);
 
    
    // Set projection matrix for shader, this won't change
        mat4 projectionMatrix = mat4(1.0f);
    
        projectionMatrix = perspective(radians(fovAngle),1024.0f / 768.0f, 0.1f,100.0f);
        mat4 viewMatrix = lookAt(cameraPosition, cameraPosition + cameraLookAt,cameraUp );
        mat4 modelMatrix = mat4(1.0f);
        mat4 modelViewProjection = projectionMatrix * viewMatrix * modelMatrix;
    
        // For frame time
        float lastFrameTime = glfwGetTime();
        double lastMousePosX, lastMousePosY;
        glfwGetCursorPos(window, &lastMousePosX, &lastMousePosY);
    
        //camera information for mouse implementation
        float cameraSpeed = 0.5f;
        float cameraFastSpeed = 2 * cameraSpeed;
        float cameraHorizontalAngle = 90.0f;
        float cameraVerticalAngle = 0.0f;

        
        // Entering Main Loop
        while(!glfwWindowShouldClose(window))
        {
      
            float dt = glfwGetTime() - lastFrameTime;
            lastFrameTime += dt;
            bool fastCam = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS;
            float currentCameraSpeed = (fastCam) ? cameraFastSpeed : cameraSpeed;


            //setting up the MVP of the world so I can place our objects within
             modelViewProjection = projectionMatrix * viewMatrix * modelMatrix;
            
            
            
      
            // set the background color to the greenish grey
            glClearColor(0.2f, 0.29f, 0.29f,1.0f);
    
            // clear the color and depth buffer at the beginning of each loop
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
            
            //set world view for nonTexture
            glUseProgram(nonTexturedShader);
            GLuint view1_noTexture = glGetUniformLocation(nonTexturedShader, "view");
            GLuint projection2_noTexture = glGetUniformLocation(nonTexturedShader, "projection");

            glUniformMatrix4fv(view1_noTexture, 1, GL_FALSE, &viewMatrix[0][0]);
            glUniformMatrix4fv(projection2_noTexture, 1, GL_FALSE, &projectionMatrix[0][0]);
            
             glUseProgram(textureShader);
            GLuint view1 = glGetUniformLocation(textureShader, "view");
            GLuint projection2 = glGetUniformLocation(textureShader, "projection");

            glUniformMatrix4fv(view1, 1, GL_FALSE, &viewMatrix[0][0]);
            glUniformMatrix4fv(projection2, 1, GL_FALSE, &projectionMatrix[0][0]);
            
            
            
            GLuint lightPositionTexture = glGetUniformLocation(textureShader, "lightPos");
             GLuint viewPositionTexture = glGetUniformLocation(textureShader, "viewPos");
            glUniform3f(lightPositionTexture, lightpos.x,lightpos.y,lightpos.z);
            glUniform3f(viewPositionTexture, cameraPosition.x,cameraPosition.y,cameraPosition.z);

                      
         
            
             //geometry for grid
           
            //set shader
            
            //get the MVP of the grid from the shader
            GLuint modelViewProjection_XZ_GRID_Texture = glGetUniformLocation(textureShader, "mvp");
             GLuint gridcolor = glGetUniformLocation(textureShader, "color");
            GLuint lightColor = glGetUniformLocation(textureShader, "lightColor");
            glUniform3f(lightColor, 1.0f,1.0f,1.0f);

            //no texture
            if (!textureOn) {
            glEnableVertexAttribArray(0);
            glBindVertexArray(vertexArrayObjects[5]);
            glUniform1ui(glGetUniformLocation(textureShader, "textureOn"), 0);
            

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
         glEnableVertexAttribArray(0);
        glBindVertexArray(vertexArrayObjects[0]);
            glUniform1i(glGetUniformLocation(textureShader, "textureOn"), 1);
            glActiveTexture(GL_TEXTURE0);
            GLuint textureLocation = glGetUniformLocation(textureShader, "textureSampler");
            glBindTexture(GL_TEXTURE_2D, snowTextureID);
            glUniform1i(textureLocation, 0);

            
            // Draw grid
            for(int j=-50; j<=50; ++j)
                    {
                        for(int i=-50; i<=50; ++i)
                                {
                                    mat4 grid = translate(mat4(1.0f), vec3(i, 0.f, j));
                                     glBindTexture(GL_TEXTURE_2D, snowTextureID);
                                    glUniformMatrix4fv(modelViewProjection_XZ_GRID_Texture, 1, GL_FALSE, &grid[0][0]);
                                    glDrawArrays(GL_TRIANGLES, 0, numOfVerticesGrid);
                                }
                    }
            }
     
            
        

 
            //geometry for the XYZ coord
            
       
                  
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
            for(int i =0; i<3;i++) {
                glUniform3f(XYZ_color, XYZ_Colors[i].x,XYZ_Colors[i].y,XYZ_Colors[i].z);
                glDrawArrays(GL_LINES, XYZ_vertexIndex, 2);
                XYZ_vertexIndex = XYZ_vertexIndex +2;
                      }


            //lamp (light source)
            glEnableVertexAttribArray(0);
            glBindVertexArray(vertexArrayObjects[4]);
            glUseProgram(lamp_Shader);
            
            mat4 WorldView_lamp = projectionMatrix * viewMatrix * modelMatrix * translate(mat4(1.0f),lightpos) * scale(mat4(1.0f),vec3(3.0f,3.0f,3.0f));
            GLuint lampshader_MVP = glGetUniformLocation(lamp_Shader, "mvp");
            glUniformMatrix4fv(lampshader_MVP, 1, GL_FALSE, &WorldView_lamp[0][0]);
            glDrawArrays(primativeRender, 0, 36);
            
            
            
            //geometry for the olaf
            
            glEnableVertexAttribArray(0);
            glBindVertexArray(vertexArrayObjects[3]);
            glUseProgram(nonTexturedShader);
            
            
            
           GLuint lightPosition = glGetUniformLocation(nonTexturedShader, "lightPos");
           glUniform3f(lightPosition, lightpos.x,lightpos.y,lightpos.z);
            GLuint viewPosition = glGetUniformLocation(nonTexturedShader, "viewPos");
                     
            glUniform3f(viewPosition, cameraPosition.x,cameraPosition.y,cameraPosition.z);
         
            glFrontFace(GL_CW);

            //get the worldview of the olaf within the scene
            mat4 WorldView_Olaf =  translateOlaf  * rotateOlaf * scaleOlaf;
            
            
            
            //get the mvp of the olaf and the olaf uniform color variable so we can use it to make eyes, buttons, gloves, nose, scarf.
            GLuint modelViewProjection_Olaf = glGetUniformLocation(nonTexturedShader, "mvp");
            GLuint olaf_Color = glGetUniformLocation(nonTexturedShader, "olaf_color");
            
          
            
             GLuint lightColorNonTexture = glGetUniformLocation(nonTexturedShader, "lightColor");
            glUniform3f(lightColorNonTexture, 1.0f,1.0f,1.0f);
            
            
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
            
            
            //switch back to cube vertices
            glEnableVertexAttribArray(0);
            glBindVertexArray(vertexArrayObjects[2]);
       
            
            //right leg
            olaf_Body = WorldView_Olaf * translate(mat4(1.0f), vec3(-0.0f, 1.3f, -0.5f)) * scale(mat4(1.0f), vec3(1.0f, 2.5f, -0.4f));
            glUniformMatrix4fv(modelViewProjection_Olaf, 1, GL_FALSE, &olaf_Body[0][0]);
            glUniform3f(olaf_Color, 1.0f,1.0f,1.0f);
            glDrawArrays(primativeRender, 0, 36);
            
            //left leg
            olaf_Body = WorldView_Olaf * translate(mat4(1.0f), vec3(-0.0f, 1.3f, 0.5f)) * scale(mat4(1.0f), vec3(1.0f, 2.5f, -0.4f));
            glUniformMatrix4fv(modelViewProjection_Olaf, 1, GL_FALSE, &olaf_Body[0][0]);
            glUniform3f(olaf_Color, 1.0f,1.0f,1.0f);
            glDrawArrays(primativeRender, 0, 36);
            
            
            //left arm
            olaf_Body = WorldView_Olaf * translate(mat4(1.0f), vec3(-0.0f, 4.7f, 1.50f)) * rotate(mat4(1.0f), radians(90.0f), vec3(6.0f, 0.0f, 0.0f)) * scale(mat4(1.0f), vec3(0.2f, 1.8f, -0.2f));
            glUniformMatrix4fv(modelViewProjection_Olaf, 1, GL_FALSE, &olaf_Body[0][0]);
            glUniform3f(olaf_Color, 1.0f,1.0f,1.0f);
            glDrawArrays(primativeRender, 0, 36);
            
            //right arm
            olaf_Body = WorldView_Olaf * translate(mat4(1.0f), vec3(-0.0f, 4.7f, -1.50f)) * rotate(mat4(1.0f), radians(90.0f), vec3(6.0f, 0.0f, 0.0f)) * scale(mat4(1.0f), vec3(0.2f, 1.8f, -0.2f));
            glUniformMatrix4fv(modelViewProjection_Olaf, 1, GL_FALSE, &olaf_Body[0][0]);
            glUniform3f(olaf_Color, 1.0f,1.0f,1.0f);
            glDrawArrays(primativeRender, 0, 36);
            
    
            
            //nose
            if (!textureOn) {
            
            glUseProgram(textureShader);
            GLuint olaf_Color_noTexture = glGetUniformLocation(textureShader, "color");
            glUniform3f(viewPositionTexture, cameraPosition.x,cameraPosition.y,cameraPosition.z);
            GLuint modelViewProjection_Olaf_Texture = glGetUniformLocation(textureShader, "mvp");
                
            
            
            olaf_Body = WorldView_Olaf * translate(mat4(1.0f), vec3(-0.896f, 5.9f, 0.0f)) * rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 0.0f, 6.0f)) * scale(mat4(1.0f), vec3(-0.09f, 0.35f, -0.1f));
            glUniformMatrix4fv(modelViewProjection_Olaf_Texture, 1, GL_FALSE, &olaf_Body[0][0]);
            glUniform3f(olaf_Color_noTexture, 1.0f,0.64f,0.0f);
            glDrawArrays(primativeRender, 0, 36);
            }
            
            
            //nose textured
            
            if(textureOn) {
            glUseProgram(textureShader);
            glActiveTexture(GL_TEXTURE0);
            GLuint textureLocation = glGetUniformLocation(textureShader, "textureSampler");
            glBindTexture(GL_TEXTURE_2D, carrotTextureID);
            glUniform1i(textureLocation, 0);
            GLuint modelViewProjection_Olaf_Texture = glGetUniformLocation(textureShader, "mvp");
        
            
            
            olaf_Body = WorldView_Olaf * translate(mat4(1.0f), vec3(-0.896f, 5.9f, 0.0f)) * rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 0.0f, 6.0f)) * scale(mat4(1.0f), vec3(-0.09f, 0.35f, -0.1f));
            glUniformMatrix4fv(modelViewProjection_Olaf_Texture, 1, GL_FALSE, &olaf_Body[0][0]);
            glBindTexture(GL_TEXTURE_2D, carrotTextureID);
            glDrawArrays(primativeRender, 0, 36);
            }
            
            glEnableVertexAttribArray(0);
            glBindVertexArray(vertexArrayObjects[3]);
            glUseProgram(nonTexturedShader);
                       
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
            olaf_Body = WorldView_Olaf * translate(mat4(1.0f), vec3(-1.0f, 4.4f, 0.0f)) * rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 0.0f, 6.0f)) * scale(mat4(1.0f), vec3(-0.1f, 0.03f, -0.1f));
            glUniformMatrix4fv(modelViewProjection_Olaf, 1, GL_FALSE, &olaf_Body[0][0]);
            glUniform3f(olaf_Color, 0.39f,0.26f,0.13f);
            glDrawArrays(primativeRender1, 0, numOfVerticesSphere);
            
            //buttons
            olaf_Body = WorldView_Olaf * translate(mat4(1.0f), vec3(-1.087f, 4.0f, 0.0f)) * rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 0.0f, 6.0f)) * scale(mat4(1.0f), vec3(-0.1f, 0.03f, -0.1f));
            glUniformMatrix4fv(modelViewProjection_Olaf, 1, GL_FALSE, &olaf_Body[0][0]);
            glUniform3f(olaf_Color, 0.39f,0.26f,0.13f);
            glDrawArrays(primativeRender1, 0, numOfVerticesSphere);
                    
            
            //buttons
            olaf_Body = WorldView_Olaf * translate(mat4(1.0f), vec3(-1.087f, 3.6f, 0.0f)) * rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 0.0f, 6.0f)) * scale(mat4(1.0f), vec3(-0.1f, 0.03f, -0.1f));
            glUniformMatrix4fv(modelViewProjection_Olaf, 1, GL_FALSE, &olaf_Body[0][0]);
            glUniform3f(olaf_Color, 0.39f,0.26f,0.13f);
            glDrawArrays(primativeRender1, 0, numOfVerticesSphere);
                    
            //buttons
            olaf_Body = WorldView_Olaf * translate(mat4(1.0f), vec3(-1.093f, 3.2f, 0.0f)) * rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 0.0f, 6.0f)) * scale(mat4(1.0f), vec3(-0.1f, 0.03f, -0.1f));
            glUniformMatrix4fv(modelViewProjection_Olaf, 1, GL_FALSE, &olaf_Body[0][0]);
            glUniform3f(olaf_Color, 0.39f,0.26f,0.13f);
            glDrawArrays(primativeRender1, 0, numOfVerticesSphere);
            
            
            //left glove
            olaf_Body = WorldView_Olaf * translate(mat4(1.0f), vec3(0.0f, 4.7f, -2.5f)) * rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 0.0f, 6.0f)) * scale(mat4(1.0f), vec3(-0.3f, -0.3f, -0.3f));
            glUniformMatrix4fv(modelViewProjection_Olaf, 1, GL_FALSE, &olaf_Body[0][0]);
               glUniform3f(olaf_Color, 1.0f,0.0f,0.0f);
            glDrawArrays(primativeRender1, 0, numOfVerticesSphere);
            
            
            //right glove
            olaf_Body = WorldView_Olaf * translate(mat4(1.0f), vec3(0.0f, 4.7f, 2.5f)) * rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 0.0f, 6.0f)) * scale(mat4(1.0f), vec3(-0.3f, -0.3f, -0.3f));
            glUniformMatrix4fv(modelViewProjection_Olaf, 1, GL_FALSE, &olaf_Body[0][0]);
               glUniform3f(olaf_Color, 1.0f,0.0f,0.0f);
            glDrawArrays(primativeRender1, 0, numOfVerticesSphere);
            
            
            
            //scarf
            olaf_Body = WorldView_Olaf * translate(mat4(1.0f), vec3(0.0f, 5.45f, 0.0f)) * rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 0.0f, 6.0f)) * scale(mat4(1.0f), vec3(0.15f, 1.0f, 0.8f));
            glUniformMatrix4fv(modelViewProjection_Olaf, 1, GL_FALSE, &olaf_Body[0][0]);
            glUniform3f(olaf_Color, 1.0f,0.0f,0.0f);
            glDrawArrays(primativeRender1, 0, numOfVerticesSphere);
            
            

            
            glEnableVertexAttribArray(0);
            glBindVertexArray(vertexArrayObjects[2]);
         
            
            glFrontFace(GL_CCW);
            
            //hat brim
            if(!textureOn) {
            glUseProgram(textureShader);
            GLuint olaf_Color_noTexture = glGetUniformLocation(textureShader, "color");
            glUniform3f(viewPositionTexture, cameraPosition.x,cameraPosition.y,cameraPosition.z);
            GLuint modelViewProjection_Olaf_Texture = glGetUniformLocation(textureShader, "mvp");

                
            olaf_Body = WorldView_Olaf * translate(mat4(1.0f), vec3(-0.0f, 6.50f, 0.f)) * rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 0.0f, 6.0f)) * scale(mat4(1.0f), vec3(0.1f, 1.2f, 1.75f));
            glUniformMatrix4fv(modelViewProjection_Olaf_Texture, 1, GL_FALSE, &olaf_Body[0][0]);
            glUniform3f(olaf_Color_noTexture, 0.0f,0.0f,0.0f);
            glDrawArrays(primativeRender, 0, 36);

            //hat top
            olaf_Body = WorldView_Olaf * translate(mat4(1.0f), vec3(-0.0f, 6.70f, 0.f)) * rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 0.0f, 6.0f)) * scale(mat4(1.0f), vec3(0.7f, 1.2f, 1.4f));
            glUniformMatrix4fv(modelViewProjection_Olaf_Texture, 1, GL_FALSE, &olaf_Body[0][0]);
            glUniform3f(olaf_Color_noTexture, 0.0f,0.0f,0.0f);
            glDrawArrays(primativeRender, 0, 36);
            }
            
            //texture hat brim
            if (textureOn) {
            glUseProgram(textureShader);
                                     glUniform3f(viewPositionTexture, cameraPosition.x,cameraPosition.y,cameraPosition.z);
            glActiveTexture(GL_TEXTURE0);
            GLuint textureLocation = glGetUniformLocation(textureShader, "textureSampler");
            glBindTexture(GL_TEXTURE_2D, shinyMetalTextureID);
            glUniform1i(textureLocation, 0);
            GLuint modelViewProjection_Olaf_Texture = glGetUniformLocation(textureShader, "mvp");
                       
            
            olaf_Body = WorldView_Olaf * translate(mat4(1.0f), vec3(-0.0f, 6.50f, 0.f)) * rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 0.0f, 6.0f)) * scale(mat4(1.0f), vec3(0.1f, 1.2f, 1.75f));
            glUniformMatrix4fv(modelViewProjection_Olaf_Texture, 1, GL_FALSE, &olaf_Body[0][0]);
            glBindTexture(GL_TEXTURE_2D, shinyMetalTextureID);
            glDrawArrays(primativeRender, 0, 36);
                       
            //textured hat top
            olaf_Body = WorldView_Olaf * translate(mat4(1.0f), vec3(-0.0f, 6.8f, 0.f)) * rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 0.0f, 6.0f)) * scale(mat4(1.0f), vec3(0.7f, 1.2f, 1.4f));
            glUniformMatrix4fv(modelViewProjection_Olaf_Texture, 1, GL_FALSE, &olaf_Body[0][0]);
            glBindTexture(GL_TEXTURE_2D, shinyMetalTextureID);
            glDrawArrays(primativeRender, 0, 36);
            
            }
            


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
                  translateOlaf = translateOlaf * translate(mat4(1.0f), vec3(xTrans, 0.0f, 0.0f));
                }
              
            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) // move olaf to the right
                {
                  float xTrans = 0.1;
                  xTrans = xTrans + 0.001f;
                  translateOlaf = translateOlaf * translate(mat4(1.0f), vec3(xTrans,0.0f, 0.0f));
                }
              
            if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) // move olaf up
                {
                  float zTrans = 0.1;
                  zTrans = zTrans + 0.001f;
                  translateOlaf = translateOlaf * translate(mat4(1.0f), vec3(0.0f,0.0f, zTrans));
                }
              
            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) // move olaf down
                {
                  float zTrans = -0.1;
                  zTrans = zTrans + 0.001f;
                  translateOlaf = translateOlaf * translate(mat4(1.0f), vec3(0.0f,0.0f, zTrans));
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
                viewMatrix = viewMatrix * rotate(mat4(1.0f), radians(x),vec3(1.0f,0.f, 0.f));
                projectionMatrix = projectionMatrix * rotate(mat4(1.0f), radians(x),vec3(1.0f,0.f, 0.f));

                               
                     }
            
            if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) // rotate X axis in the other orientation of the world
                             {
                                                                
                float x =5.01;
                float y=5.01;
                float z=5.01;
                x+=0.01;
                y+=0.01;
                z+=0.01;
                viewMatrix = viewMatrix * rotate(mat4(1.0f), radians(x),vec3(-1.0f,0.f, 0.f));
                             
                projectionMatrix = projectionMatrix * rotate(mat4(1.0f), radians(x),vec3(-1.0f,0.f, 0.f));

                                     
                             }
                                        
            
            
            
            if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) //rotate Y axis of the world
                             {
                                                                
                float x =5.01;
                float y=5.01;
                float z=5.01;
                x+=0.01;
                y+=0.01;
                z+=0.01;
                modelMatrix = modelMatrix * rotate(mat4(1.0f), radians(x),vec3(0.0f,1.f, 0.f));
                             }
            
            
            if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) // rotate Y axis of the world in the other orientation
                             {
                                                                
                float x =5.01;
                float y=5.01;
                float z=5.01;
                x+=0.01;
                y+=0.01;
                z+=0.01;
                modelMatrix = modelMatrix * rotate(mat4(1.0f), radians(x),vec3(0.0f,-1.0f, 0.f));
                             }
            
            if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS) // reset world orientation to original settings
                            {
                                                                         
              float x =5.01;
              float y=5.01;
              float z=5.01;
              x+=0.01;
              y+=0.01;
              z+=0.01;
              modelMatrix = mat4(1.0f);
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
     }
   
    
    
   
    
       
    
