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
// 월드 변환과 뷰-투영 행렬을 위한 uniform
uniform mat4 uWorldTransform;
uniform mat4 uViewProj;

// Attribute 0 is position, 1 is tex coords.
// 속성 0은 위치 좌표, 1은 텍스처 좌표
// layout 명령어는 속성 슬롯이 어떤 변수에 해당하는지를 지정
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTexCoord;

// Add texture coordinate as output
// 텍스처 좌표를 출력에 추가
out vec2 fragTexCoord;

void main()
{
	// Convert position to homogeneous coordinates
	// 위지 좌표를 동차 좌표로 변환
	vec4 pos = vec4(inPosition, 1.0);
	// Transform position to world space, then clip space
	// 위치를 월드 좌표로 변환 뒤 클립 공간 좌표로 변환
	gl_Position = pos * uWorldTransform * uViewProj;

	// Transform
	// Pass along the texture coordinate to frag shader
	// 텍스처 좌표를 프래그먼트 셰이더에 전달
	fragTexCoord = inTexCoord;
}
