#version 330 core

uniform mat4 matTransform;

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec4 inColour;
layout (location = 2) in vec2 inTexUVScene;
layout (location = 3) in vec2 inTexUVLight;

out vec4 vertColour;
out vec2 vertUVScene;
out vec2 vertUVLight;

void main()
{
    gl_Position = matTransform*vec4(inPosition, 1.0);
    vertUVScene = inTexUVScene;
    vertUVLight = inTexUVLight;
    vertColour = inColour;
}
