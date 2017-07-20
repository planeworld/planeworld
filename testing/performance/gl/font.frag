#version 330 core

uniform sampler2D FontTexture;
in  vec2          vertUV;
in  vec4          vertColour;

out vec4          outColour;

void main()
{
    vec4 c = texture(FontTexture, vertUV);
    outColour = vec4(vertColour.r,
                     vertColour.g,
                     vertColour.b,
                     vertColour.a * c.r);
}
