#version 330 core

in vec3 color;
out vec4 thecolor;

void main()
{
    thecolor = vec4(color,1.0f);
}