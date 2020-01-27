// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

// Request GLSL 3.3 GLSL 3.3 버전 요청
#version 330

// Uniforms for world transform and view-proj
// 월드 변환 및 뷰-투영 행렬을 위한 uniform
uniform mat4 uWorldTransform;
uniform mat4 uViewProj;

// Vertex attributes
// 버텍스 속성
in vec3 inPosition;

void main()
{
	vec4 pos = vec4(inPosition, 1.0);
	gl_Position = pos * uWorldTransform * uViewProj;
}
