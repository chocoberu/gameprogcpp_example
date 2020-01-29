// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include <vector>
#include <string>

class Mesh
{
public:
	Mesh();
	~Mesh();
	// Load/unload mesh
	// �޽� �ε�/��ε�
	bool Load(const std::string& fileName, class Renderer* renderer);
	void Unload();
	// Get the vertex array associated with this mesh
	// �� �޽ÿ� �ش��ϴ� ���ؽ� �迭�� ��´�
	class VertexArray* GetVertexArray() { return mVertexArray; }
	// Get a texture from specified index
	// Ư�� �ε����� �ش��ϴ� �ؽ�ó�� ��´�
	class Texture* GetTexture(size_t index);
	// Get name of shader
	// ���̴��� �̸��� ��´�
	const std::string& GetShaderName() const { return mShaderName; }
	// Get object space bounding sphere radius
	// ������Ʈ ���� �ٿ�� ��ü �������� ��´�
	float GetRadius() const { return mRadius; }
	// Get specular power of mesh
	// 
	float GetSpecPower() const { return mSpecPower; }
private:
	// Textures associated with this mesh
	// �� �޽ÿ��� ���Ǵ� �ؽ�ó��
	std::vector<class Texture*> mTextures;
	// Vertex array associated with this mesh
	// �޽��� ���ؽ� �迭
	class VertexArray* mVertexArray;
	// Name of shader specified by mesh
	// �޽ð� ������ ���̴� �̸�
	std::string mShaderName;
	// Stores object space bounding sphere radius
	// ������Ʈ ���� �ٿ�� ��ü�� �������� ����
	float mRadius;
	// Specular power of surface
	// �޽� ǥ���� ���ݻ� ����
	float mSpecPower;
};