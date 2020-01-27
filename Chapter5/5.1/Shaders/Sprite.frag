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
// ���ؽ� ���̴����� ��� �ؽ�ó ��ǥ �Է°�
in vec2 fragTexCoord;

// This corresponds to the output color to the color buffer
// ��� ����
out vec4 outColor;

// This is used for the texture sampling
// �ؽ�ó ���ø��� ���� �ʿ�
uniform sampler2D uTexture;

void main()
{
	// Sample color from texture
	// �ؽ�ó�κ��� ������ ���ø�
	outColor = texture(uTexture, fragTexCoord);
}
