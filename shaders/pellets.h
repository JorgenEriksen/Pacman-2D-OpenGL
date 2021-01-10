#pragma once

#include <string>

const std::string pelletVertexShaderSrc = R"(
#version 430 core

layout(location = 0) in vec4 position;
layout(location = 1) in float alive;

out float alive2;

void main()
{
alive2 = alive;
gl_Position = position;
gl_PointSize = 10.0f;
}
)";
// Fragment shader code
const std::string pelletFragmentShaderSrc = R"(
#version 430 core

in float alive2;
out vec4 color;

void main()
{
	color = vec4(1.0f, 1.0f, 1.0f, alive2);

	vec2 circleCoordinates = 2.0f * gl_PointCoord - 1.0f;
    if (dot(circleCoordinates, circleCoordinates) > 1.0f) {
    discard;
}
}
)";
