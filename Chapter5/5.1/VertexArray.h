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
	// 이 버텍스 배열 (정점 배열)을 활성화 (활성화를 하면 이 버텍스 배열을 그릴 수가 있다)
	void SetActive();

	unsigned int GetNumIndices() const { return mNumIndices; }
	unsigned int GetNumVerts() const { return mNumVerts; }
private:
	// How many vertices in the vertex buffer?
	// 버텍스 버퍼에 존재하는 버텍스 수
	unsigned int mNumVerts;
	// How many indices in the index buffer
	// 인덱스 버퍼에 존재하는 인덱스의 수
	unsigned int mNumIndices;
	// OpenGL ID of the vertex buffer
	// 버텍스 버퍼의 OpenGL ID
	unsigned int mVertexBuffer;
	// OpenGL ID of the index buffer
	// 인덱스 버퍼의 OpenGL ID
	unsigned int mIndexBuffer;
	// OpenGL ID of the vertex array object
	// 버텍스 배열 개체의 OpenGL ID
	unsigned int mVertexArray;
};