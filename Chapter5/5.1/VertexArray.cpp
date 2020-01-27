// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "VertexArray.h"
#include <GL/glew.h>

// 버텍스 배열 개체를 생성한 후 mVertexArray 멤버 변수에 ID 저장
VertexArray::VertexArray(const float* verts, unsigned int numVerts,
	const unsigned int* indices, unsigned int numIndices)
	:mNumVerts(numVerts)
	,mNumIndices(numIndices)
{
	// Create vertex array
	// 버텍스 배열 생성
	// 버택스 배열 개체를 사용해서 버텍스 버퍼,인덱스 버퍼, 버텍스 레이아웃을 캡슐화
	// 버텍스 레이아웃은 모델의 각 버텍스에 저장할 데이터를 지정한다
	glGenVertexArrays(1, &mVertexArray);
	glBindVertexArray(mVertexArray); // mVertexArray에 OpenGL의 버텍스 배열 개체의 id 저장

	// Create vertex buffer
	// 이후 버텍스 버퍼 생성
	glGenBuffers(1, &mVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer); // 버텍스 버퍼를 버퍼로 사용한다 (GL_ARRAY_BUFFER)
	glBufferData(GL_ARRAY_BUFFER, // 데이터를 쓸 버퍼의 버퍼 타입
		numVerts * 5 * sizeof(float), // 복사할 바이트 크기
		verts, // 복사할 소스 (포인터)
		GL_STATIC_DRAW); // 이 데이터를 어떻게 사용할 것인가

	// Create index buffer
	// 인덱스 버퍼 생성
	glGenBuffers(1, &mIndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, // 인덱스 버퍼
		numIndices * sizeof(unsigned int), // 데이터의 크기
		indices, 
		GL_STATIC_DRAW);

	// Specify the vertex attributes
	// (For now, assume one vertex format)
	// Position is 3 floats starting at offset 0
	// 첫번째 버텍스 속성 (속성 0)을 활성화
	glEnableVertexAttribArray(0);
	// 크기와 타입, 속성의 포맷 지정
	glVertexAttribPointer(0, // 속성 인덱스 (첫 번째 버텍스 속성 인덱스는 0)
		3, // 요소의 수
		GL_FLOAT, // 요소의 타입
		GL_FALSE, // (정수형 타입에서만 사용됨)
		sizeof(float) * 5, // 간격 (일반적으로 각 버텍스의 크기)
		0); // 버텍스의 시작에서 이 속성까지의 오프셋
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, // 속성 인덱스, 2번째 버텍스 속성인 텍스처 좌표의 속성 인덱스
		2, // 컴포넌트의 수 (UV, 2)
		GL_FLOAT, // 각 컴포넌트의 타입
		GL_FALSE, // GL_FLOAT에서는 사용되지 않음
		sizeof(float) * 5, // 간격 (간격은 항상 버텍스의 크기)
		reinterpret_cast<void*>(sizeof(float) * 3)); // 오프셋 포인터
}

VertexArray::~VertexArray()
{
	// 버텍스 버퍼, 인덱스 버퍼, 버텍스 배열 객체를 해제
	glDeleteBuffers(1, &mVertexBuffer);
	glDeleteBuffers(1, &mIndexBuffer);
	glDeleteVertexArrays(1, &mVertexArray);
}

void VertexArray::SetActive()
{
	// 현재 사용할 버텍스 배열을 지정
	glBindVertexArray(mVertexArray);
}
