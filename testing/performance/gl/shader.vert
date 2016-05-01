#version 330 core

uniform mat4 matProjection;

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inColour;

out vec3 vertColour;

void main()
{
   gl_Position = matProjection*vec4(inPosition, 1.0);
   vertColour = inColour;
}
