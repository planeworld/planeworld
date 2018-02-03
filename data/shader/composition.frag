#version 330 core

uniform sampler2D SceneTexture;
uniform sampler2D LightsTexture;
in  vec2          vertUVScene;
in  vec2          vertUVLight;
in  vec4          vertColour;

out vec4          outColour;

void main()
{
    vec4 vecTmp = texture(SceneTexture, vertUVScene);
    outColour = vec4(vecTmp.rgb, 1.0);
}
