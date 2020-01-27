// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

// Request GLSL 3.3
#version 330

// Uniforms for world transform and view-proj
// ���� ��ȯ�� ��-���� ����� ���� uniform
uniform mat4 uWorldTransform;
uniform mat4 uViewProj;

// Attribute 0 is position, 1 is tex coords.
// �Ӽ� 0�� ��ġ ��ǥ, 1�� �ؽ�ó ��ǥ
// layout ��ɾ�� �Ӽ� ������ � ������ �ش��ϴ����� ����
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTexCoord;

// Add texture coordinate as output
// �ؽ�ó ��ǥ�� ��¿� �߰�
out vec2 fragTexCoord;

void main()
{
	// Convert position to homogeneous coordinates
	// ���� ��ǥ�� ���� ��ǥ�� ��ȯ
	vec4 pos = vec4(inPosition, 1.0);
	// Transform position to world space, then clip space
	// ��ġ�� ���� ��ǥ�� ��ȯ �� Ŭ�� ���� ��ǥ�� ��ȯ
	gl_Position = pos * uWorldTransform * uViewProj;

	// Transform
	// Pass along the texture coordinate to frag shader
	// �ؽ�ó ��ǥ�� �����׸�Ʈ ���̴��� ����
	fragTexCoord = inTexCoord;
}
