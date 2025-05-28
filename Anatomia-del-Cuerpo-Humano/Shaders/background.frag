#version 330 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D background;

void main()
{
    vec4 texColor = texture(background, TexCoords);

    // Si la textura no tiene alpha, fuerza alfa completo
    if (texColor.a == 0.0)
        texColor.a = 1.0;

    FragColor = texColor;
}
