#version 460 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 ourTexCoordinates;
vec2 faceTexCoordinates;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float mixValue;

void main()
{
    faceTexCoordinates = vec2(-ourTexCoordinates.x, ourTexCoordinates.y);
	FragColor = mix(texture(texture1, ourTexCoordinates), texture(texture2, ourTexCoordinates), mixValue);
}