// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "VertexArray.h"
#include <GL/glew.h>

// ���ؽ� �迭 ��ü�� ������ �� mVertexArray ��� ������ ID ����
VertexArray::VertexArray(const float* verts, unsigned int numVerts,
	const unsigned int* indices, unsigned int numIndices)
	:mNumVerts(numVerts)
	,mNumIndices(numIndices)
{
	// Create vertex array
	// ���ؽ� �迭 ����
	// ���ý� �迭 ��ü�� ����ؼ� ���ؽ� ����,�ε��� ����, ���ؽ� ���̾ƿ��� ĸ��ȭ
	// ���ؽ� ���̾ƿ��� ���� �� ���ؽ��� ������ �����͸� �����Ѵ�
	glGenVertexArrays(1, &mVertexArray);
	glBindVertexArray(mVertexArray); // mVertexArray�� OpenGL�� ���ؽ� �迭 ��ü�� id ����

	// Create vertex buffer
	// ���� ���ؽ� ���� ����
	glGenBuffers(1, &mVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer); // ���ؽ� ���۸� ���۷� ����Ѵ� (GL_ARRAY_BUFFER)
	glBufferData(GL_ARRAY_BUFFER, // �����͸� �� ������ ���� Ÿ��
		numVerts * 5 * sizeof(float), // ������ ����Ʈ ũ��
		verts, // ������ �ҽ� (������)
		GL_STATIC_DRAW); // �� �����͸� ��� ����� ���ΰ�

	// Create index buffer
	// �ε��� ���� ����
	glGenBuffers(1, &mIndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, // �ε��� ����
		numIndices * sizeof(unsigned int), // �������� ũ��
		indices, 
		GL_STATIC_DRAW);

	// Specify the vertex attributes
	// (For now, assume one vertex format)
	// Position is 3 floats starting at offset 0
	// ù��° ���ؽ� �Ӽ� (�Ӽ� 0)�� Ȱ��ȭ
	glEnableVertexAttribArray(0);
	// ũ��� Ÿ��, �Ӽ��� ���� ����
	glVertexAttribPointer(0, // �Ӽ� �ε��� (ù ��° ���ؽ� �Ӽ� �ε����� 0)
		3, // ����� ��
		GL_FLOAT, // ����� Ÿ��
		GL_FALSE, // (������ Ÿ�Կ����� ����)
		sizeof(float) * 5, // ���� (�Ϲ������� �� ���ؽ��� ũ��)
		0); // ���ؽ��� ���ۿ��� �� �Ӽ������� ������
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, // �Ӽ� �ε���, 2��° ���ؽ� �Ӽ��� �ؽ�ó ��ǥ�� �Ӽ� �ε���
		2, // ������Ʈ�� �� (UV, 2)
		GL_FLOAT, // �� ������Ʈ�� Ÿ��
		GL_FALSE, // GL_FLOAT������ ������ ����
		sizeof(float) * 5, // ���� (������ �׻� ���ؽ��� ũ��)
		reinterpret_cast<void*>(sizeof(float) * 3)); // ������ ������
}

VertexArray::~VertexArray()
{
	// ���ؽ� ����, �ε��� ����, ���ؽ� �迭 ��ü�� ����
	glDeleteBuffers(1, &mVertexBuffer);
	glDeleteBuffers(1, &mIndexBuffer);
	glDeleteVertexArrays(1, &mVertexArray);
}

void VertexArray::SetActive()
{
	// ���� ����� ���ؽ� �迭�� ����
	glBindVertexArray(mVertexArray);
}
