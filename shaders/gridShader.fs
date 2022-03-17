#version 330 core
in vec3 ourColor;

out vec4 FragColor;

void main(){
    // gl_FragColor = vec4(ourColor, 1.0);
    FragColor = vec4(ourColor, 1.0);
}
