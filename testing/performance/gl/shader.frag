#version 330

smooth in vec3 theColor;
out vec4 outputColor;

void main()
{
   outputColor = vec4(theColor, 1.0);
}
