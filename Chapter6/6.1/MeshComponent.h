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

class MeshComponent : public Component // 3D 메시를 그리기 위한 컴포넌트, 메시와 텍스처 인덱스 정보를 가지고 있다
{
public:
	MeshComponent(class Actor* owner);
	~MeshComponent();
	// Draw this mesh component
	// 메시 컴포넌트를 그린다
	virtual void Draw(class Shader* shader);
	// Set the mesh/texture index used by mesh component
	// 메시 컴포넌트가 사용하는 메시/텍스처를 설정
	virtual void SetMesh(class Mesh* mesh) { mMesh = mesh; }
	void SetTextureIndex(size_t index) { mTextureIndex = index; }
	const class Mesh* GetMesh() const { return mMesh; } // 6.1을 위해 추가한 함수
protected:
	class Mesh* mMesh; // 메시 클래스에 버택스 배열 객체, 텍스처 배열, 셰이더 이름 등이 저장됨
	size_t mTextureIndex; // 텍스처는 메시 클래스에 저장됨
};
