#version 330 core

in vec2 vUV;

uniform sampler2D uTexture;
uniform vec2 uAtlasSize;       // Atlas dimensions in pixels
uniform vec4 uAmbientColor;    // Day/night + region tint (r, g, b, a)
uniform float uTime;           // Seconds — for animated tiles

out vec4 FragColor;

void main()
{
    vec4 texel = texture(uTexture, vUV);

    if (texel.a < 0.01)
        discard;

    // Apply ambient lighting (day/night cycle, region mood, torch glow)
    FragColor = texel * uAmbientColor;
}