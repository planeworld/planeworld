#version 330 core

in vec4 vertColour;

layout(location = 0) out vec4 outColour;

void main()
{
   outColour = vertColour;
}
