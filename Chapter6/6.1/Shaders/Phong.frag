// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

// Request GLSL 3.3
// GLSL 3.3 버전 요청
#version 330

// Inputs from vertex shader
// Tex coord
// 버텍스 셰이더에서 오는 입력값
// 텍스처 좌표
in vec2 fragTexCoord;
// Normal (in world space)
// 노말 벡터 (월드 공간 기준)
in vec3 fragNormal;
// Position (in world space)
// 위치 좌표 (월드 공간 기준)
in vec3 fragWorldPos;

// This corresponds to the output color to the color buffer
// 출력값 색상
out vec4 outColor;

// This is used for the texture sampling
// 텍스처 샘플링을 위해 사용하는 변수
uniform sampler2D uTexture;

// Create a struct for directional light
// 방향광을 위한 구조체 정의
struct DirectionalLight
{
	// Direction of light
	// 방향광
	vec3 mDirection;
	// Diffuse color
	// 난반사 색상
	vec3 mDiffuseColor;
	// Specular color
	// 정반사 색상
	vec3 mSpecColor;
};

// Uniforms for lighting
// Camera position (in world space)
// 조명을 위한 uniform
// 월드 공간에서 카메라 위치
uniform vec3 uCameraPos;
// Specular power for this surface
// 표면에 대한 정반사 지수
uniform float uSpecPower;
// Ambient light level
// 주변광
uniform vec3 uAmbientLight;

// Directional Light
// 방향광 (지금은 오직 하나)
uniform DirectionalLight uDirLight;

void main()
{
	// Surface normal
	// 표면 노멀벡터 정규화
	vec3 N = normalize(fragNormal);
	// Vector from surface to light
	// 표면에서 광원으로의 벡터
	vec3 L = normalize(-uDirLight.mDirection);
	// Vector from surface to camera
	// 표면에서 카메라로 향하는 벡터
	vec3 V = normalize(uCameraPos - fragWorldPos);
	// Reflection of -L about N
	// N에 대한 -L의 반사
	vec3 R = normalize(reflect(-L, N));

	// Compute phong reflection
	// 퐁 반사 계산
	vec3 Phong = uAmbientLight;
	float NdotL = dot(N, L);
	if (NdotL > 0)
	{
		vec3 Diffuse = uDirLight.mDiffuseColor * NdotL;
		vec3 Specular = uDirLight.mSpecColor * pow(max(0.0, dot(R, V)), uSpecPower);
		Phong += Diffuse + Specular;
	}

	// Final color is texture color times phong light (alpha = 1)
	// 최종색은 텍스처 색상 * 퐁 광원
    outColor = texture(uTexture, fragTexCoord) * vec4(Phong, 1.0f);
}
