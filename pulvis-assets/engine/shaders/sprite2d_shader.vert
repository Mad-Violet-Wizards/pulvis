#version 460 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aUV;

uniform mat4 uMVP;
uniform vec4 uUVRect; // x, y, w, h in [0..1]

out vec2 vUV;

void main()
{
    // Engine texture space is top-left; GL samples bottom-left.
    // Flip the per-quad UV (0..1) BEFORE mapping into the atlas sub-rect,
    // so the flip happens inside the frame and does not permute atlas rows.
    vec2 base_uv = vec2(aUV.x, 1.0 - aUV.y);
    vUV = uUVRect.xy + base_uv * uUVRect.zw;
    gl_Position = uMVP * vec4(aPos, 0.0, 1.0);
}