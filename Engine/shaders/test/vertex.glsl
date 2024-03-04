#version 410 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 texCoord;

out vec4 vertexColor;
out vec2 vertexTexCoord;

uniform vec4 uCameraPos;
uniform vec4 uCameraRotation;
uniform vec4 uCameraRotRadians4;
uniform float uCameraRotRadians;

vec4 RotatePoint(vec4 point, float radians)
{
	float deg = degrees(radians);

	float sinus = sin(deg);
	float cosinus = cos(deg);

	float x = point.x;
	float y = point.y;
	float z = point.z;
	float w = point.w;

	float xp = x * cosinus - y * sinus;
	float yp = x * sinus + y * cosinus;

	vec4 newPoint = vec4(xp, yp, z, w);

	return newPoint;
}

vec4 Rotate3D(vec4 point, vec4 radians)
{
	vec3 point_ = vec3(point.x, point.y, point.w);
	vec3 result = point_;

	if (radians.x != 0)
	{
		float deg = degrees(radians.x);
		mat3 rotMatrix;
		rotMatrix[0] = vec3(1.0, 0.0, 0.0);
		rotMatrix[1] = vec3(0.0, cos(deg), -sin(deg));
		rotMatrix[2] = vec3(0.0, sin(deg), cos(deg));

		result = result + rotMatrix * point_;
	}
	if (radians.y != 0)
	{
		float deg = degrees(radians.y);
		mat3 rotMatrix;
		rotMatrix[0] = vec3(cos(deg), 0.0, sin(deg));
		rotMatrix[1] = vec3(0.0, 1.0, 0.0);
		rotMatrix[2] = vec3(-sin(deg), 0.0, cos(deg));

		result = result + rotMatrix * point_;
	}
	if (radians.z != 0)
	{
		float deg = degrees(radians.z);
		mat3 rotMatrix;
		rotMatrix[0] = vec3(cos(deg), -sin(deg), 0.0);
		rotMatrix[1] = vec3(sin(deg), cos(deg), 0.0);
		rotMatrix[2] = vec3(0.0, 0.0, 1.0);

		result = result + rotMatrix * point_;
	}

	vec4 newPoint = vec4(result.x, result.y, point.z, result.z);
	return newPoint;
}

void main()
{
	vertexColor = color;
	vertexTexCoord = texCoord;

	gl_Position = position + uCameraPos;
	gl_Position = Rotate3D(gl_Position, uCameraRotRadians4);
}