// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

// Request GLSL 3.3 GLSL 3.3 버전 요청
#version 330

// This should correspond to the data stored
// for each vertex in the vertex buffer.
// For now, just a position.
// 모든 버텍스 속성은 여기에
// 현재 버텍스 속성은 위치 하나 뿐
in vec3 inPosition;

void main()
{
	// The vertex shader needs to output a 4D
	// coordinate.
	// For now set the 4th coordinate to 1.0
	// inPosition을 gl_Position으로 직접 전달
	// 버텍스 셰이더는 4d 좌표가 필요, 그러므로 vec3에서 vec4로 변환
	gl_Position = vec4(inPosition, 1.0);
}
