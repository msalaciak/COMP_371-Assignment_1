//
// COMP 371 Assignment 1
//
// Created by Matthew Salaciak 29644490.
//
// Inspired by the COMP 371 Lab 2,3 and 4 and the following tutorials:
// - https://learnopengl.com/Getting-started/Hello-Triangle
// - https://learnopengl.com/Advanced-OpenGL/Advanced-GLSL (for shader class)


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

using namespace glm;
using namespace std;



int main(int argc, char*argv[])
{
    mat4 translateOlaf(1.f);
    mat4 scaleOlaf(1.f);
    srand (time(NULL));
    int primativeRender = GL_TRIANGLES;
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
    glfwMakeContextCurrent(window);
    glViewport(0, 0, 1024, 768);

    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to create GLEW" << std::endl;
        glfwTerminate();
        return -1;
    }
    

      
      //grid shader
    GLuint XZ_grid_shader = Shader("/Users/matthew/Documents/school/WINTER 2020/COMP 371/assignments/A1_29644490/Assignment1_Framework/Source/grid-shader.vs", "/Users/matthew/Documents/school/WINTER 2020/COMP 371/assignments/A1_29644490/Assignment1_Framework/Source/grid-shader.fs");
    
    GLuint XYZ_Shader = Shader("/Users/matthew/Documents/school/WINTER 2020/COMP 371/assignments/A1_29644490/Assignment1_Framework/Source/xyz-shader.vs", "/Users/matthew/Documents/school/WINTER 2020/COMP 371/assignments/A1_29644490/Assignment1_Framework/Source/xyz-shader.fs");
    
    GLuint olaf_Shader = Shader("/Users/matthew/Documents/school/WINTER 2020/COMP 371/assignments/A1_29644490/Assignment1_Framework/Source/olaf-shader.vs", "/Users/matthew/Documents/school/WINTER 2020/COMP 371/assignments/A1_29644490/Assignment1_Framework/Source/olaf-shader.fs");
    
      
    
      
       glUseProgram(XZ_grid_shader);


        
        // Define and upload geometry to the GPU here ...
          GLuint vertexArrayObjects[3], vertexBufferObjects[3];
          glGenVertexArrays(3, vertexArrayObjects);
          glGenBuffers(3, vertexBufferObjects);
          
          //bind grid vertices
          glBindVertexArray(vertexArrayObjects[0]);
          glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjects[0]);
          glBufferData(GL_ARRAY_BUFFER, sizeof(grid_vertices), grid_vertices, GL_STATIC_DRAW);
          glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
          
          //bind xyz vertices
          glBindVertexArray(vertexArrayObjects[1]);
          glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjects[1]);
          glBufferData(GL_ARRAY_BUFFER, sizeof(xyz_verticles), xyz_verticles, GL_STATIC_DRAW);
          glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
          
          
          glBindVertexArray(vertexArrayObjects[2]);
          glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjects[2]);
          glBufferData(GL_ARRAY_BUFFER, sizeof(snowman_vertices), snowman_vertices, GL_STATIC_DRAW);
          glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
          glEnableVertexAttribArray(0);



        
        // Other OpenGL states to set once
        // Enable Backface culling
        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
       


    
    // Camera parameters for view transform
         vec3 cameraPosition(0.0f,10.0f,30.0f);
         vec3 cameraLookAt(0.0f, 0.0f, 0.0f);
         vec3 cameraUp(0.0f, 1.0f, 0.0f);
 
    
    // Set projection matrix for shader, this won't change
                  mat4 projectionMatrix = mat4(1.0f);
                  projectionMatrix = perspective(radians(45.0f),1024.0f / 768.0f, 0.1f,100.0f);

                  mat4 viewMatrix = lookAt(cameraPosition,  // eye
                                           cameraPosition + cameraLookAt,  // center
                                           cameraUp ); // up
                 mat4 modelMatrix = mat4(1.0f);

                 mat4 modelViewProjection = projectionMatrix * viewMatrix * modelMatrix;
    
    // For frame time
    float lastFrameTime = glfwGetTime();
    int lastMouseLeftState = GLFW_RELEASE;
    double lastMousePosX, lastMousePosY;
    glfwGetCursorPos(window, &lastMousePosX, &lastMousePosY);
    
    
    float cameraSpeed = 0.5f;
    float cameraFastSpeed = 2 * cameraSpeed;
    float cameraHorizontalAngle = 90.0f;
    float cameraVerticalAngle = 0.0f;
    bool  cameraFirstPerson = true; // press 1 or 2 to toggle this variable
        
        // Entering Main Loop
        while(!glfwWindowShouldClose(window))
        {
      
            float dt = glfwGetTime() - lastFrameTime;
            lastFrameTime += dt;
            bool fastCam = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS;
            float currentCameraSpeed = (fastCam) ? cameraFastSpeed : cameraSpeed;


              
             modelViewProjection = projectionMatrix * viewMatrix * modelMatrix;
            
      
            // Black background
                glClearColor(0.2f, 0.29f, 0.29f,1.0f);
    
    
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            
            // Draw geometry for grid
             glEnableVertexAttribArray(0);
            glBindVertexArray(vertexArrayObjects[0]);
            glUseProgram(XZ_grid_shader);

            // Draw grid
         
            GLuint modelViewProjection_XZ_GRID = glGetUniformLocation(XZ_grid_shader, "mvp");
       

            for(int j=-50; j<=50; ++j)
                    {
                        for(int i=-50; i<=50; ++i)
                                {
                                    mat4 x1 = modelViewProjection * translate(mat4(1.0f), vec3(i, 0.f, j));
                                        glUniformMatrix4fv(modelViewProjection_XZ_GRID, 1, GL_FALSE, &x1[0][0]);
                                        glDrawArrays(GL_LINE_LOOP, 0, 4);
                                    mat4 z1 = modelViewProjection * translate(mat4(1.0f), vec3(j, 0.f, i));
                                        glUniformMatrix4fv(modelViewProjection_XZ_GRID, 1, GL_FALSE, &z1[0][0]);
                                        glDrawArrays(GL_LINE_LOOP, 0, 4);
                                }
                    }
            

 
            //draw XYZ coord
            
            glEnableVertexAttribArray(0);
            glBindVertexArray(vertexArrayObjects[1]);
            glUseProgram(XYZ_Shader);
            
            GLuint modelViewProjection_XYZ = glGetUniformLocation(XYZ_Shader, "mvp");
            GLuint XYZ_color = glGetUniformLocation(XYZ_Shader, "xyz_color");
            mat4 camera_ModelViewProject_XYZ =  projectionMatrix * viewMatrix * modelMatrix;
            
            glUniformMatrix4fv(modelViewProjection_XYZ, 1, GL_FALSE, &camera_ModelViewProject_XYZ[0][0]);

         
            int XYZ_vertexIndex =0;
            for(int i =0; i<3;i++) {
                glUniform3f(XYZ_color, XYZ_Colors[i].x,XYZ_Colors[i].y,XYZ_Colors[i].z);
                glDrawArrays(GL_LINES, XYZ_vertexIndex, 2);
                XYZ_vertexIndex = XYZ_vertexIndex +2;
                }


            //draw snowman
            
            glEnableVertexAttribArray(0);
            glBindVertexArray(vertexArrayObjects[2]);
            glUseProgram(olaf_Shader);
            
            mat4 rotateOlaf = rotate(mat4(1.0f), radians(90.0f),vec3(00.0f,90.0f, 0.0f));
            mat4 WorldView_Olaf = projectionMatrix * viewMatrix * translateOlaf * scaleOlaf * rotateOlaf;
            
            GLuint modelViewProjection_Olaf = glGetUniformLocation(olaf_Shader, "mvp");
            GLuint olaf_Color = glGetUniformLocation(olaf_Shader, "olaf_color");
            
            //body
            mat4 olaf_Body = WorldView_Olaf * translate(mat4(1.0f), vec3(0.f, 3.5f, 0.f))* scale(mat4(1.0f), vec3(2.f, 2.f, 2.0f));
            glUniformMatrix4fv(modelViewProjection_Olaf, 1, GL_FALSE, &olaf_Body[0][0]);
            glUniform4f(olaf_Color, 1.0f,1.0f,1.0f,1.0f);
            
            glDrawArrays(primativeRender, 0, 36);
            
            //upper body
            olaf_Body = WorldView_Olaf * translate(mat4(1.0f), vec3(0.f, 4.55f, 0.f)) * scale(mat4(1.0f), vec3(-1.5f, -1.3f, -1.6f));
            glUniformMatrix4fv(modelViewProjection_Olaf, 1, GL_FALSE, &olaf_Body[0][0]);
            glUniform4f(olaf_Color, 1.0f,1.0f,1.0f,1.0f);
            glDrawArrays(primativeRender, 0, 36);
            
            //head
            olaf_Body = WorldView_Olaf * translate(mat4(1.0f), vec3(0.f, 5.55f, 0.f)) * scale(mat4(1.0f), vec3(-0.7f, -1.3f, -1.0f));
            glUniformMatrix4fv(modelViewProjection_Olaf, 1, GL_FALSE, &olaf_Body[0][0]);
            glUniform4f(olaf_Color, 1.0f,1.0f,1.0f,1.0f);
            glDrawArrays(primativeRender, 0, 36);
            
            //right leg
            olaf_Body = WorldView_Olaf * translate(mat4(1.0f), vec3(-0.0f, 1.3f, -0.5f)) * scale(mat4(1.0f), vec3(1.0f, 2.5f, -0.4f));
            glUniformMatrix4fv(modelViewProjection_Olaf, 1, GL_FALSE, &olaf_Body[0][0]);
            glUniform4f(olaf_Color, 1.0f,1.0f,1.0f,1.0f);
            glDrawArrays(primativeRender, 0, 36);
            
            //left leg
            olaf_Body = WorldView_Olaf * translate(mat4(1.0f), vec3(-0.0f, 1.3f, 0.5f)) * scale(mat4(1.0f), vec3(1.0f, 2.5f, -0.4f));
            glUniformMatrix4fv(modelViewProjection_Olaf, 1, GL_FALSE, &olaf_Body[0][0]);
            glUniform4f(olaf_Color, 1.0f,1.0f,1.0f,1.0f);
            glDrawArrays(primativeRender, 0, 36);
            
            
            //left arm
            olaf_Body = WorldView_Olaf * translate(mat4(1.0f), vec3(-0.0f, 4.7f, 1.50f)) * rotate(mat4(1.0f), radians(90.0f), vec3(6.0f, 0.0f, 0.0f)) * scale(mat4(1.0f), vec3(0.2f, 1.8f, -0.2f));
            glUniformMatrix4fv(modelViewProjection_Olaf, 1, GL_FALSE, &olaf_Body[0][0]);
            glUniform4f(olaf_Color, 1.0f,1.0f,1.0f,1.0f);
            glDrawArrays(primativeRender, 0, 36);
            
            //right arm
            olaf_Body = WorldView_Olaf * translate(mat4(1.0f), vec3(-0.0f, 4.7f, -1.50f)) * rotate(mat4(1.0f), radians(90.0f), vec3(6.0f, 0.0f, 0.0f)) * scale(mat4(1.0f), vec3(0.2f, 1.8f, -0.2f));
            glUniformMatrix4fv(modelViewProjection_Olaf, 1, GL_FALSE, &olaf_Body[0][0]);
            glUniform4f(olaf_Color, 1.0f,1.0f,1.0f,1.0f);
            glDrawArrays(primativeRender, 0, 36);
            
            //nose
            olaf_Body = WorldView_Olaf * translate(mat4(1.0f), vec3(-0.4f, 5.7f, 0.0f)) * rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 0.0f, 6.0f)) * scale(mat4(1.0f), vec3(-0.09f, 0.5f, -0.1f));
            glUniformMatrix4fv(modelViewProjection_Olaf, 1, GL_FALSE, &olaf_Body[0][0]);
            glUniform4f(olaf_Color, 1.0f,0.64f,0.0f,1.0f);
            glDrawArrays(primativeRender, 0, 36);
            
            //left eye
            olaf_Body = WorldView_Olaf * translate(mat4(1.0f), vec3(-0.3f, 5.85f, -0.25f)) * rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 0.0f, 6.0f)) * scale(mat4(1.0f), vec3(-0.09f, 0.1f, -0.1f));
            glUniformMatrix4fv(modelViewProjection_Olaf, 1, GL_FALSE, &olaf_Body[0][0]);
            glUniform4f(olaf_Color, 0.0f,0.0f,0.0f,1.0f);
            glDrawArrays(primativeRender, 0, 36);
            
            //right eye
            olaf_Body = WorldView_Olaf * translate(mat4(1.0f), vec3(-0.3f, 5.85f, 0.25f)) * rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 0.0f, 6.0f)) * scale(mat4(1.0f), vec3(-0.09f, 0.1f, -0.1f));
            glUniformMatrix4fv(modelViewProjection_Olaf, 1, GL_FALSE, &olaf_Body[0][0]);
            glUniform4f(olaf_Color, 0.0f,0.0f,0.0f,1.0f);
            glDrawArrays(primativeRender, 0, 36);
            
            //mouth
            olaf_Body = WorldView_Olaf * translate(mat4(1.0f), vec3(-0.4f, 5.4f, 0.0f)) * rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 0.0f, 6.0f)) * scale(mat4(1.0f), vec3(-0.2f, 0.1f, -0.2f));
            glUniformMatrix4fv(modelViewProjection_Olaf, 1, GL_FALSE, &olaf_Body[0][0]);
            glUniform4f(olaf_Color, 0.0f,0.0f,0.0f,1.0f);
            glDrawArrays(primativeRender, 0, 36);
            
            //buttons
            olaf_Body = WorldView_Olaf * translate(mat4(1.0f), vec3(-1.087f, 4.4f, 0.0f)) * rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 0.0f, 6.0f)) * scale(mat4(1.0f), vec3(-0.1f, -0.1f, -0.1f));
            glUniformMatrix4fv(modelViewProjection_Olaf, 1, GL_FALSE, &olaf_Body[0][0]);
            glUniform4f(olaf_Color, 0.39f,0.26f,0.13f,1.0f);
            glDrawArrays(primativeRender, 0, 36);
            
            //buttons
            olaf_Body = WorldView_Olaf * translate(mat4(1.0f), vec3(-1.087f, 4.2f, 0.0f)) * rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 0.0f, 6.0f)) * scale(mat4(1.0f), vec3(-0.1f, -0.1f, -0.1f));
            glUniformMatrix4fv(modelViewProjection_Olaf, 1, GL_FALSE, &olaf_Body[0][0]);
            glUniform4f(olaf_Color, 0.39f,0.26f,0.13f,1.0f);
            glDrawArrays(primativeRender, 0, 36);
                    
            
            //buttons
            olaf_Body = WorldView_Olaf * translate(mat4(1.0f), vec3(-1.087f, 4.0f, 0.0f)) * rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 0.0f, 6.0f)) * scale(mat4(1.0f), vec3(-0.1f, -0.1f, -0.1f));
            glUniformMatrix4fv(modelViewProjection_Olaf, 1, GL_FALSE, &olaf_Body[0][0]);
            glUniform4f(olaf_Color, 0.39f,0.26f,0.13f,1.0f);
            glDrawArrays(primativeRender, 0, 36);
                    
            //buttons
            olaf_Body = WorldView_Olaf * translate(mat4(1.0f), vec3(-1.087f, 3.8f, 0.0f)) * rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 0.0f, 6.0f)) * scale(mat4(1.0f), vec3(-0.1f, -0.1f, -0.1f));
            glUniformMatrix4fv(modelViewProjection_Olaf, 1, GL_FALSE, &olaf_Body[0][0]);
            glUniform4f(olaf_Color, 0.39f,0.26f,0.13f,1.0f);
            glDrawArrays(primativeRender, 0, 36);
            
            //left glove
            olaf_Body = WorldView_Olaf * translate(mat4(1.0f), vec3(0.0f, 4.7f, -2.5f)) * rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 0.0f, 6.0f)) * scale(mat4(1.0f), vec3(-0.3f, -0.3f, -0.3f));
            glUniformMatrix4fv(modelViewProjection_Olaf, 1, GL_FALSE, &olaf_Body[0][0]);
            glUniform4f(olaf_Color, 0.0f,0.0f,0.0f,1.0f);
            glDrawArrays(primativeRender, 0, 36);
            
            
            //right glove
            olaf_Body = WorldView_Olaf * translate(mat4(1.0f), vec3(0.0f, 4.7f, 2.5f)) * rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 0.0f, 6.0f)) * scale(mat4(1.0f), vec3(-0.3f, -0.3f, -0.3f));
            glUniformMatrix4fv(modelViewProjection_Olaf, 1, GL_FALSE, &olaf_Body[0][0]);
            glUniform4f(olaf_Color, 0.0f,0.0f,0.0f,1.0f);
            glDrawArrays(primativeRender, 0, 36);
           
          
            
            
            
            
            // End Frame
            glfwSwapBuffers(window);
            glfwPollEvents();
            
            
            
            
            
            
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
                       
                       // Clamp vertical angle to [-85, 85] degrees
                       cameraVerticalAngle = std::max(-85.0f, std::min(85.0f, cameraVerticalAngle));
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

            
      if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) // move camera to the left
      {
          cameraPosition.x -= currentCameraSpeed * dt*40;
      }
      
      if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) // move camera to the right
      {
          cameraPosition.x += currentCameraSpeed * dt*40;
      }
      
      if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) // move camera up
      {
          cameraPosition.y -= currentCameraSpeed * dt*40;
      }
      
      if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) // move camera down
      {
          cameraPosition.y += currentCameraSpeed * dt*40;
      }
            
      if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) // move camera up
               {
                   cameraPosition.z -= currentCameraSpeed * dt*40;
               }
               
               if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) // move camera down
               {
                   cameraPosition.z += currentCameraSpeed * dt*40;
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
            
            
            if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) // scale down
                                   {
                                       
                                       primativeRender = GL_TRIANGLES;
                                   }
            
            if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) // scale down
                                             {
                                                 
                                                 primativeRender = GL_POINTS;
                                             }
            
            if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) // scale down
            {
                
                primativeRender = GL_LINES;
            }
            //0 is for points
            //3 is for lines
            //
            
            
            
            
            
            viewMatrix = lookAt(cameraPosition, cameraPosition + cameraLookAt, cameraUp );
            
         
        }

        
        // Shutdown GLFW
        glfwTerminate();
        
        return 0;
    }


    
     
   
    
    
   
    
       
    
