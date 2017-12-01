#version 330 core

uniform sampler2D ScreenTexture;
uniform float     transparency;
in  vec2          vertUV;
in  vec4          vertColour;

out vec4          outColour;

void main()
{
    vec4 vecTmp = texture(ScreenTexture, vertUV);
    outColour = vec4(vecTmp.rgb, transparency);
}
