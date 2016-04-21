#version 330 core

uniform mat4 matProjection;

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inColor;

smooth out vec3 theColor;

void main()
{
   gl_Position = matProjection*vec4(inPosition, 1.0);
   theColor = inColor;
}
