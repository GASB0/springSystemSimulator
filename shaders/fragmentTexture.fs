#version 330 core
// Información de entrada
// in vec3 ourColor;
in vec2 TexCoord;
uniform sampler2D aTexture0; // To be bound to texture unit 0
uniform sampler2D aTexture1; // To be bound to texture unit 0

// Esta es la variable de salida de este programa (los fragmentos)
out vec4 FragColor;
void main()
{
   vec4 Texture0 = texture(aTexture0, vec2(TexCoord.x, 1-TexCoord.y));
   vec4 Texture1 = texture(aTexture1, TexCoord);

  // FragColor =  texture(aTexture0, TexCoord)*vec4(ourColor, 1.0);
   FragColor = mix(Texture0, Texture1, 0.0);
}
