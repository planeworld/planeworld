#version 330 core

in  vec2          vertUV;
out vec4          outColour;
uniform sampler2D RenderedTexture;

void main()
{
    outColour = texture(RenderedTexture, vertUV);
}
