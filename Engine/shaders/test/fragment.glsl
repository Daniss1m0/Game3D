#version 410 core

out vec4 outputColor;

in vec4 vertexColor;
in vec2 vertexTexCoord;

uniform sampler2D tex;
uniform int uHasTexture;
uniform float uBrightness;

void main()
{
	if (uHasTexture == 1)
		outputColor = texture(tex, vertexTexCoord);
	else
		outputColor = vec4(vertexColor.x * uBrightness, vertexColor.g * uBrightness, vertexColor.b * uBrightness, 1.0);
}