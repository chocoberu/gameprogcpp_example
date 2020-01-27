// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

// Request GLSL 3.3 GLSL 3.3 ������ ��û
#version 330

// This corresponds to the output color
// to the color buffer
// ��� ������ �����ϱ� ���� out ���� �����ڷ� ���� ���� ����
out vec4 outColor; // RGBA ���� ������ 4�� ��ҿ� �ش��ϴ� vec4 Ÿ�� ����

void main() // ���⿡ ���̴� �ڵ� �ۼ�
{
	// RGBA of 100% blue, 100% opaque
	// Ǫ�� ������ ����
    outColor = vec4(0.0, 0.0, 1.0, 1.0);
}
