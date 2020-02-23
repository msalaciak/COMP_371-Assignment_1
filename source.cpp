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

//define namespaces for glm and c++ std
using namespace glm;
using namespace std;



int main(int argc, char*argv[])
{
    
    //define our identity matrix's for translate, rotate and scale olaf, this is used later on when I implement the key binding fuctions to control the olafs movements
    mat4 translateOlaf(1.f);
    mat4 scaleOlaf(1.f);
    mat4 rotateOlaf = rotate(mat4(1.0f), radians(90.0f),vec3(00.0f,90.0f, 0.0f));
    
    //random number initialization and int primativeRender is used to store the default rendering option which is GL_TRIANGLES
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
    
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwMakeContextCurrent(window);
    glViewport(0, 0, 1024, 768);

    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to create GLEW" << std::endl;
        glfwTerminate();
        return -1;
    }
    

      
     // the following functions loads the shader files and creates my shader objects for the XZ grid, the XYZ coordinate lines ans the olaf
    GLuint XZ_grid_shader = Shader("/Users/matthew/Documents/school/WINTER 2020/COMP 371/assignments/A1_29644490/Assignment1_Framework/Source/grid-shader.vs", "/Users/matthew/Documents/school/WINTER 2020/COMP 371/assignments/A1_29644490/Assignment1_Framework/Source/grid-shader.fs");
    
    GLuint XYZ_Shader = Shader("/Users/matthew/Documents/school/WINTER 2020/COMP 371/assignments/A1_29644490/Assignment1_Framework/Source/xyz-shader.vs", "/Users/matthew/Documents/school/WINTER 2020/COMP 371/assignments/A1_29644490/Assignment1_Framework/Source/xyz-shader.fs");
    
    GLuint olaf_Shader = Shader("/Users/matthew/Documents/school/WINTER 2020/COMP 371/assignments/A1_29644490/Assignment1_Framework/Source/olaf-shader.vs", "/Users/matthew/Documents/school/WINTER 2020/COMP 371/assignments/A1_29644490/Assignment1_Framework/Source/olaf-shader.fs");
    
      

      

    
        // Define and upload geometry to the GPU here by creating a VAO and VBO that has the size of 3
        // This way we can store of the geometry of all three objects at different indices.
    
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
          
          //bind olaf vertices
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

            
            //geometry for grid
             glEnableVertexAttribArray(0);
            glBindVertexArray(vertexArrayObjects[0]);
            glUseProgram(XZ_grid_shader);

           
         
            //get the MVP of the grid from the shader
            GLuint modelViewProjection_XZ_GRID = glGetUniformLocation(XZ_grid_shader, "mvp");
            // Draw grid
            for(int j=-50; j<=50; ++j)
                    {
                        for(int i=-50; i<=50; ++i)
                                {
                                    mat4 grid = modelViewProjection * translate(mat4(1.0f), vec3(i, 0.f, j));
                                    glUniformMatrix4fv(modelViewProjection_XZ_GRID, 1, GL_FALSE, &grid[0][0]);
                                    glDrawArrays(GL_LINE_LOOP, 0, 4);
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


            //geometry for the olaf
            
            glEnableVertexAttribArray(0);
            glBindVertexArray(vertexArrayObjects[2]);
            glUseProgram(olaf_Shader);
            
            
            //get the worldview of the olaf within the scene
            mat4 WorldView_Olaf = projectionMatrix * viewMatrix * translateOlaf * scaleOlaf * rotateOlaf;
            
            //get the mvp of the olaf and the olaf uniform color variable so we can use it to make eyes, buttons, gloves, nose, scarf.
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
            olaf_Body = WorldView_Olaf * translate(mat4(1.0f), vec3(-0.55f, 5.7f, 0.0f)) * rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 0.0f, 6.0f)) * scale(mat4(1.0f), vec3(-0.09f, 0.35f, -0.1f));
            glUniformMatrix4fv(modelViewProjection_Olaf, 1, GL_FALSE, &olaf_Body[0][0]);
            glUniform4f(olaf_Color, 1.0f,0.64f,0.0f,1.0f);
            glDrawArrays(primativeRender, 0, 36);
            
            //left eye
            olaf_Body = WorldView_Olaf * translate(mat4(1.0f), vec3(-0.4f, 5.85f, -0.2f)) * rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 0.0f, 6.0f)) * scale(mat4(1.0f), vec3(-0.09f, 0.07f, -0.1f));
            glUniformMatrix4fv(modelViewProjection_Olaf, 1, GL_FALSE, &olaf_Body[0][0]);
            glUniform4f(olaf_Color, 0.0f,0.0f,0.0f,1.0f);
            glDrawArrays(primativeRender, 0, 36);
            
            //right eye
            olaf_Body = WorldView_Olaf * translate(mat4(1.0f), vec3(-0.4f, 5.85f, 0.2f)) * rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 0.0f, 6.0f)) * scale(mat4(1.0f), vec3(-0.09f, 0.07f, -0.1f));
            glUniformMatrix4fv(modelViewProjection_Olaf, 1, GL_FALSE, &olaf_Body[0][0]);
            glUniform4f(olaf_Color, 0.0f,0.0f,0.0f,1.0f);
            glDrawArrays(primativeRender, 0, 36);
            
            //mouth
            olaf_Body = WorldView_Olaf * translate(mat4(1.0f), vec3(-0.4f, 5.4f, 0.0f)) * rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 0.0f, 6.0f)) * scale(mat4(1.0f), vec3(-0.2f, 0.07f, -0.2f));
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
            
            
            //scarf left side
            olaf_Body = WorldView_Olaf * translate(mat4(1.0f), vec3(-0.02f, 5.15f, -0.85f)) * rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 0.0f, 6.0f)) * scale(mat4(1.0f), vec3(-0.1f, -1.70f, -0.1f));
            glUniformMatrix4fv(modelViewProjection_Olaf, 1, GL_FALSE, &olaf_Body[0][0]);
            glUniform4f(olaf_Color, 1.0f,0.0f,0.0f,1.0f);
                glDrawArrays(primativeRender, 0, 36);
            
            //scarf right side
            olaf_Body = WorldView_Olaf * translate(mat4(1.0f), vec3(0.0f, 5.15f, 0.85f)) * rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 0.0f, 6.0f)) * scale(mat4(1.0f), vec3(-0.1f, -1.75f, -0.1f));
            glUniformMatrix4fv(modelViewProjection_Olaf, 1, GL_FALSE, &olaf_Body[0][0]);
            glUniform4f(olaf_Color, 1.0f,0.0f,0.0f,1.0f);
            glDrawArrays(primativeRender, 0, 36);
            
            //scarf back side
            olaf_Body = WorldView_Olaf * translate(mat4(1.0f), vec3(0.8f, 5.15f, 0.0f)) * rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 0.0f, 6.0f)) * scale(mat4(1.0f), vec3(-0.1f, -0.15f, 1.8f));
            glUniformMatrix4fv(modelViewProjection_Olaf, 1, GL_FALSE, &olaf_Body[0][0]);
            glUniform4f(olaf_Color, 1.0f,0.0f,0.0f,1.0f);
            glDrawArrays(primativeRender, 0, 36);
            
            
            
            //scarf overhang
            olaf_Body = WorldView_Olaf * translate(mat4(1.0f), vec3(-0.8f, 5.15f, 0.35f)) * rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 0.0f, 6.0f)) * scale(mat4(1.0f), vec3(-0.1f, -0.15f, 2.3f));
            glUniformMatrix4fv(modelViewProjection_Olaf, 1, GL_FALSE, &olaf_Body[0][0]);
            glUniform4f(olaf_Color, 1.0f,0.0f,0.0f,1.0f);
            glDrawArrays(primativeRender, 0, 36);
            
            //hat brim
            olaf_Body = WorldView_Olaf * translate(mat4(1.0f), vec3(-0.0f, 6.25f, 0.f)) * rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 0.0f, 6.0f)) * scale(mat4(1.0f), vec3(-0.1f, -0.5f, 1.75f));
            glUniformMatrix4fv(modelViewProjection_Olaf, 1, GL_FALSE, &olaf_Body[0][0]);
            glUniform4f(olaf_Color, 0.0f,0.0f,0.0f,1.0f);
            glDrawArrays(primativeRender, 0, 36);
            
            //hat top
            olaf_Body = WorldView_Olaf * translate(mat4(1.0f), vec3(-0.0f, 6.55f, 0.f)) * rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 0.0f, 6.0f)) * scale(mat4(1.0f), vec3(0.5f, -0.5f, 1.4f));
            glUniformMatrix4fv(modelViewProjection_Olaf, 1, GL_FALSE, &olaf_Body[0][0]);
            glUniform4f(olaf_Color, 0.0f,0.0f,0.0f,1.0f);
            glDrawArrays(primativeRender, 0, 36);
            
            
           
          
            
            
            
            
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

            
            
            //these are the following keybindings to control the olaf, the camera and the world orientation
            
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
                    cameraPosition.z -= currentCameraSpeed * dt*40;
                }
                                  
            if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) // camera zoom out
                {
                    cameraPosition.z += currentCameraSpeed * dt*40;
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
                }
            
            if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) // primative change to points
                {
                    primativeRender = GL_POINTS;
                }
            
            if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) // primative change to lines
                {
                primativeRender = GL_LINES;
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
                modelMatrix = modelMatrix * rotate(mat4(1.0f), radians(x),vec3(1.0f,0.f, 0.f));
                     }
            
            if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) // rotate X axis in the other orientation of the world
                             {
                                                                
                float x =5.01;
                float y=5.01;
                float z=5.01;
                x+=0.01;
                y+=0.01;
                z+=0.01;
                modelMatrix = modelMatrix * rotate(mat4(1.0f), radians(x),vec3(-1.0f,0.f, 0.f));
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
            
         
        }

        
        // Shutdown GLFW
        glfwTerminate();
        
        return 0;
    }


    
     
   
    
    
   
    
       
    
