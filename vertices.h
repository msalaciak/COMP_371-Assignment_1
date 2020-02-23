//
//  vertices.h
//  A1_29644490
//
// COMP 371 Assignment 1
//
// Created by Matthew Salaciak 29644490.
//

#include <iostream>
#include <list>
#define GLEW_STATIC 1
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>

using namespace glm;
using namespace std;


static const vec3 grid_vertices[] =
{
    vec3(0.0f, 0.0f, 0.0f),
    vec3(1.0f, 0.0f, 0.0f),

    vec3(1.0f, 0.0f, 1.0f),
    vec3(0.0f, 0.0f, 1.0f),


};




static const vec3 xyz_verticles [] = {
  
    //x
         vec3(0.0f, 0.5f,  0.0f),
         vec3(5.0f, 0.5f,  0.0f),
  
    //y
         vec3(0.0f, 0.0f,  0.0f),
         vec3( 0.0f, 5.0f,  0.0f),

    //z
         vec3(0.0f, 0.5f,  0.0f),
         vec3(0.0f, 0.5f,  5.0f),
    
};


static const vec3 XYZ_Colors[] =
{
    vec3(1.0f, 0.0f, 0.0f),
    vec3(0.0f, 1.0f, 0.0f),
    vec3(0.0f, 0.0f, 1.0)
    
  
};




  // //cube model to use for snowman
static const vec3 snowman_vertices[] = {
    vec3(-0.5f,-0.5f,-0.5f),
    vec3(-0.5f,-0.5f, 0.5f),
    vec3(-0.5f, 0.5f, 0.5f),
    
    vec3(-0.5f,-0.5f,-0.5f),
    vec3(-0.5f, 0.5f, 0.5f),
    vec3(-0.5f, 0.5f,-0.5f),
    
    vec3( 0.5f, 0.5f,-0.5f),
    vec3(-0.5f,-0.5f,-0.5f),
    vec3(-0.5f, 0.5f,-0.5f),
    
    vec3( 0.5f, 0.5f,-0.5f),
    vec3( 0.5f,-0.5f,-0.5f),
    vec3(-0.5f,-0.5f,-0.5f),
    
    vec3( 0.5f,-0.5f, 0.5f),
    vec3(-0.5f,-0.5f,-0.5f),
    vec3( 0.5f,-0.5f,-0.5f),
    
    vec3( 0.5f,-0.5f, 0.5f),
    vec3(-0.5f,-0.5f, 0.5f),
    vec3(-0.5f,-0.5f,-0.5f),
    
    vec3(-0.5f, 0.5f, 0.5f),
    vec3(-0.5f,-0.5f, 0.5f),
    vec3( 0.5f,-0.5f, 0.5f),
    
    vec3( 0.5f, 0.5f, 0.5f),
    vec3(-0.5f, 0.5f, 0.5f),
    vec3( 0.5f,-0.5f, 0.5f),
    
    vec3( 0.5f, 0.5f, 0.5f),
    vec3( 0.5f,-0.5f,-0.5f),
    vec3( 0.5f, 0.5f,-0.5f),
    
    vec3( 0.5f,-0.5f,-0.5f),
    vec3( 0.5f, 0.5f, 0.5f),
    vec3( 0.5f,-0.5f, 0.5f),
    
    vec3( 0.5f, 0.5f, 0.5f),
    vec3( 0.5f, 0.5f,-0.5f),
    vec3(-0.5f, 0.5f,-0.5f),
    
    vec3( 0.5f, 0.5f, 0.5f),
    vec3(-0.5f, 0.5f,-0.5f),
    vec3(-0.5f, 0.5f, 0.5f)

  };


