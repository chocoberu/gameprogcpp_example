// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

// Request GLSL 3.3
// GLSL 3.3 ���� ��û
#version 330

// Inputs from vertex shader
// Tex coord
// ���ؽ� ���̴����� ���� �Է°�
// �ؽ�ó ��ǥ
in vec2 fragTexCoord;
// Normal (in world space)
// �븻 ���� (���� ���� ����)
in vec3 fragNormal;
// Position (in world space)
// ��ġ ��ǥ (���� ���� ����)
in vec3 fragWorldPos;

// This corresponds to the output color to the color buffer
// ��°� ����
out vec4 outColor;

// This is used for the texture sampling
// �ؽ�ó ���ø��� ���� ����ϴ� ����
uniform sampler2D uTexture;

// Create a struct for directional light
// ���Ɽ�� ���� ����ü ����
struct DirectionalLight
{
	// Direction of light
	// ���Ɽ
	vec3 mDirection;
	// Diffuse color
	// ���ݻ� ����
	vec3 mDiffuseColor;
	// Specular color
	// ���ݻ� ����
	vec3 mSpecColor;
};

// Uniforms for lighting
// Camera position (in world space)
// ������ ���� uniform
// ���� �������� ī�޶� ��ġ
uniform vec3 uCameraPos;
// Specular power for this surface
// ǥ�鿡 ���� ���ݻ� ����
uniform float uSpecPower;
// Ambient light level
// �ֺ���
uniform vec3 uAmbientLight;

// Directional Light
// ���Ɽ (������ ���� �ϳ�)
uniform DirectionalLight uDirLight;

void main()
{
	// Surface normal
	// ǥ�� ��ֺ��� ����ȭ
	vec3 N = normalize(fragNormal);
	// Vector from surface to light
	// ǥ�鿡�� ���������� ����
	vec3 L = normalize(-uDirLight.mDirection);
	// Vector from surface to camera
	// ǥ�鿡�� ī�޶�� ���ϴ� ����
	vec3 V = normalize(uCameraPos - fragWorldPos);
	// Reflection of -L about N
	// N�� ���� -L�� �ݻ�
	vec3 R = normalize(reflect(-L, N));

	// Compute phong reflection
	// �� �ݻ� ���
	vec3 Phong = uAmbientLight;
	float NdotL = dot(N, L);
	if (NdotL > 0)
	{
		vec3 Diffuse = uDirLight.mDiffuseColor * NdotL;
		vec3 Specular = uDirLight.mSpecColor * pow(max(0.0, dot(R, V)), uSpecPower);
		Phong += Diffuse + Specular;
	}

	// Final color is texture color times phong light (alpha = 1)
	// �������� �ؽ�ó ���� * �� ����
    outColor = texture(uTexture, fragTexCoord) * vec4(Phong, 1.0f);
}
