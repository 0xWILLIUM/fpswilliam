#version 330 core
layout (location = 0) in vec3 aPos;

out vec4 vertexColor;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 color;
void main()
{
    // gl_Position = vec4(aPos, 1.0f);
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
    // ourColor = aColor;
    vertexColor = vec4(color, 1.0f);
}