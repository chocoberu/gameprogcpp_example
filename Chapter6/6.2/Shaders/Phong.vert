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

// Attribute 0 is position, 1 is normal, 2 is tex coords.
// �Ӽ� 0�� ��ġ, 1�� ��ֺ���, 2�� �ؽ�ó ��ǥ
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

// Any vertex outputs (other than position)
// ���ؽ� ���
out vec2 fragTexCoord;
// Normal (in world space)
// ���� ���������� ��� ����
out vec3 fragNormal;
// Position (in world space)
// ���� ���������� ��ġ ��ǥ
out vec3 fragWorldPos;

void main()
{
	// Convert position to homogeneous coordinates
	// ��ġ�� ���� ��ǥ�� ��ȯ
	vec4 pos = vec4(inPosition, 1.0);
	// Transform position to world space
	// ��ġ�� ���� �������� ��ġ�� ��ȯ
	pos = pos * uWorldTransform;
	// Save world position
	// ���� �������� ��ġ�� ����
	fragWorldPos = pos.xyz;
	// Transform to clip space
	// ��ġ�� Ŭ�� ���� ��ǥ�� ��ȯ
	gl_Position = pos * uViewProj;

	// Transform normal into world space (w = 0)
	// ��� ���͸� ���� �������� ��� ���ͷ� ��ȯ (w = 0)
	fragNormal = (vec4(inNormal, 0.0f) * uWorldTransform).xyz;

	// Pass along the texture coordinate to frag shader
	// �ؽ�ó ��ǥ�� �����׸�Ʈ ���̴��� ����
	fragTexCoord = inTexCoord;
}
