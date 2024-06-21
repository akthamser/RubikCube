#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 acolor;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in float aface;

out vec3 color;
out vec2 TexCoord;
flat out int face;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    color = acolor;
    TexCoord = aTexCoord;
    face = int(aface);
}