#version 330 core

uniform mat4 matTransform;

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec4 inColour;
layout (location = 2) in vec2 inTexUV;

out vec4 vertColour;
out vec2 vertUV;

void main()
{
    gl_Position = matTransform*vec4(inPosition, 1.0);
    vertUV = inTexUV;
    vertColour = inColour;
}
