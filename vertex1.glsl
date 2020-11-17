#version 330

in vec3 vPos;
in vec3 vColor;

out vec3 passColor;

uniform mat4 trans;


void main(void)
{
		gl_Position = trans*vec4(vPos, 1);
		passColor = vColor;
}
