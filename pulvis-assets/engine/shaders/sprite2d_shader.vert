#version 460 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aUV;

uniform mat4 uMVP;
uniform vec4 uUVRect; // x, y, w, h in [0..1]

out vec2 vUV;

void main()
{
    vUV = uUVRect.xy + aUV * uUVRect.zw;
    gl_Position = uMVP * vec4(aPos, 0.0, 1.0);
}