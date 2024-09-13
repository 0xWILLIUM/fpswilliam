#version 330 core
out vec4 FragColor;

in vec4 vertexColor;

void main()
{
    // FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
    FragColor = vertexColor;
}