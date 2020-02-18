#version 330 core

        layout (location = 0) in vec3 aPos;
        
       uniform mat4 mvp;
       uniform mat4 model;
       uniform mat4 view;
       uniform mat4 projection;
       in vec4 color;
        out vec4 vertexColor;

       void main()
       {
           gl_Position = mvp * vec4(aPos, 1.0f);
           vertexColor = color;
       }
        
