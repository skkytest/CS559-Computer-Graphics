#version 330 core

uniform sampler2D tex;

out vec4 color;

void main()
{
	ivec2 tex_size = textureSize(tex, 1);
	float size = tex_size.x * tex_size.y;
	vec4 in_color = texture(tex, gl_FragCoord.xy / size);
    color = in_color;
}