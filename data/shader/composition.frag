#version 330 core

uniform sampler2D SceneTexture;
uniform sampler2D LightsTexture;
in  vec2          vertUVScene;
in  vec2          vertUVLights;
in  vec4          vertColour;

out vec4          outColour;

void main()
{
    vec4 Scene = texture(SceneTexture, vertUVScene);
    vec4 Light = texture(LightsTexture, vertUVLights);
    outColour = vec4(Scene.rgb*Light.rgb*0.5+
                     Scene.rgb*0.8+
                     Light.rgb*0.1, 1.0);
}
