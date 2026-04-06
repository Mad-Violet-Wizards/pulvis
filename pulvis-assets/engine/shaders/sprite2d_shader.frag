#version 460 core

in vec2 vUV;

uniform sampler2D uTexture;
uniform vec4 uColor; // tint + opacity (r, g, b, a)

out vec4 FragColor;

void main()
{
    vec4 texel = texture(uTexture, vUV);

    // Discard fully transparent pixels
    if (texel.a < 0.01)
        discard;

    FragColor = texel * uColor;
}