#version 330 core
// Información de entrada
layout (location = 0) in vec3 aPos;
// layout (location = 1) in vec3 aColor;
layout (location = 1) in vec2 aTexCoord;

// Uniformes
uniform mat4 projection;
uniform mat4 view;
uniform mat4 modelTransform;

// Información de salida
out vec3 ourColor;
out vec2 TexCoord;

void main()
{
    // gl_Position = transform*vec4(aPos, 1.0);
    gl_Position = projection*view*modelTransform*vec4(aPos, 1.0);
    TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}
