// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

// Request GLSL 3.3 GLSL 3.3 ���� ��û
#version 330

// Uniforms for world transform and view-proj
// ���� ��ȯ �� ��-���� ����� ���� uniform
uniform mat4 uWorldTransform;
uniform mat4 uViewProj;

// Vertex attributes
// ���ؽ� �Ӽ�
in vec3 inPosition;

void main()
{
	vec4 pos = vec4(inPosition, 1.0);
	gl_Position = pos * uWorldTransform * uViewProj;
}
