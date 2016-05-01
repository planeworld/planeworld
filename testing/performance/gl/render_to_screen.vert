#version 330 core

uniform mat4 matProjection;

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inTexUV;

out vec2 vertUV;

void main()
{
    gl_Position = matProjection*vec4(inPosition, 1.0);
    vertUV = inTexUV;
}
