// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

// Request GLSL 3.3 GLSL 3.3 버전을 요청
#version 330

// This corresponds to the output color
// to the color buffer
// 출력 색상을 저장하기 위해 out 변수 지정자로 전역 변수 선언
out vec4 outColor; // RGBA 색상 버퍼의 4개 요소에 해당하는 vec4 타입 변수

void main() // 여기에 셰이더 코드 작성
{
	// RGBA of 100% blue, 100% opaque
	// 푸른 색으로 설정
    outColor = vec4(0.0, 0.0, 1.0, 1.0);
}
