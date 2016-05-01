#version 330 core

in vec3 vertColour;

layout(location = 0) out vec4 outColour;

void main()
{
   outColour = vec4(vertColour, 1.0);
}
