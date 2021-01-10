#shader vertex

#version 430 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoord;

uniform mat4 u_ProjectionMat = mat4(1);
uniform mat4 model = mat4(1);

void main()
{
	gl_Position = model * u_ProjectionMat * vec4(a_Position, 1.0f);
	v_TexCoord = texCoord;
};

#shader fragment
#version 430 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main()
{
	vec4 texColor = texture(u_Texture, v_TexCoord);
	color = texColor;
};