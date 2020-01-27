// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

// Request GLSL 3.3
#version 330

// Tex coord input from vertex shader
// 버텍스 셰이더에서 얻는 텍스처 좌표 입력값
in vec2 fragTexCoord;
in vec3 fragVerColor; // 버텍스 색상, 버텍스 셰이더에서 그대로 옮김

// This corresponds to the output color to the color buffer
// 출력 색상
out vec4 outColor;

// This is used for the texture sampling
// 텍스처 샘플링을 위해 필요
uniform sampler2D uTexture;

void main()
{
	// Sample color from texture
	// 텍스처로부터 색상을 샘플링
	outColor = texture(uTexture, fragTexCoord);
	vec4 verColor = vec4(fragVerColor, 0.0);
	outColor += verColor;
	outColor /= 2.0;
}
