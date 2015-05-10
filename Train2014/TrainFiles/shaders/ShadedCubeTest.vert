 #version 400 core
 in vec3 position;
 in vec3 color;
 in vec2 textCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 ourColor;
out vec2 TexCoord;

void main()
{
   gl_Position = projection * view *model * vec4(position, 1.0f);
   ourColor = color;
   TexCoord = textCoord;
}
