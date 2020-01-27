// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
class VertexArray
{
public:
	VertexArray(const float* verts, unsigned int numVerts,
		const unsigned int* indices, unsigned int numIndices);
	~VertexArray();

	// Activate this vertex array (so we can draw it)
	// �� ���ؽ� �迭 (���� �迭)�� Ȱ��ȭ (Ȱ��ȭ�� �ϸ� �� ���ؽ� �迭�� �׸� ���� �ִ�)
	void SetActive();

	unsigned int GetNumIndices() const { return mNumIndices; }
	unsigned int GetNumVerts() const { return mNumVerts; }
private:
	// How many vertices in the vertex buffer?
	// ���ؽ� ���ۿ� �����ϴ� ���ؽ� ��
	unsigned int mNumVerts;
	// How many indices in the index buffer
	// �ε��� ���ۿ� �����ϴ� �ε����� ��
	unsigned int mNumIndices;
	// OpenGL ID of the vertex buffer
	// ���ؽ� ������ OpenGL ID
	unsigned int mVertexBuffer;
	// OpenGL ID of the index buffer
	// �ε��� ������ OpenGL ID
	unsigned int mIndexBuffer;
	// OpenGL ID of the vertex array object
	// ���ؽ� �迭 ��ü�� OpenGL ID
	unsigned int mVertexArray;
};