#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
uniform float hDisplacement = 0; // Esto, por suerte, admite valores por defecto
out vec3 ourColor;

void main()
{
    vec3 newPos = vec3(aPos.x + hDisplacement, aPos.y, aPos.z);
    gl_Position = vec4(newPos, 1.0);
    // ourColor = aColor;
    ourColor = aPos;
}
