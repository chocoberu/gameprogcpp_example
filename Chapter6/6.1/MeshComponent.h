// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include "Component.h"
#include <cstddef>
#include <string>

class MeshComponent : public Component // 3D �޽ø� �׸��� ���� ������Ʈ, �޽ÿ� �ؽ�ó �ε��� ������ ������ �ִ�
{
public:
	MeshComponent(class Actor* owner);
	~MeshComponent();
	// Draw this mesh component
	// �޽� ������Ʈ�� �׸���
	virtual void Draw(class Shader* shader);
	// Set the mesh/texture index used by mesh component
	// �޽� ������Ʈ�� ����ϴ� �޽�/�ؽ�ó�� ����
	virtual void SetMesh(class Mesh* mesh) { mMesh = mesh; }
	void SetTextureIndex(size_t index) { mTextureIndex = index; }
	const class Mesh* GetMesh() const { return mMesh; } // 6.1�� ���� �߰��� �Լ�
protected:
	class Mesh* mMesh; // �޽� Ŭ������ ���ý� �迭 ��ü, �ؽ�ó �迭, ���̴� �̸� ���� �����
	size_t mTextureIndex; // �ؽ�ó�� �޽� Ŭ������ �����
};
