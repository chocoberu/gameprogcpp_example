// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "MeshComponent.h"
#include "Shader.h"
#include "Mesh.h"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"
#include "Texture.h"
#include "VertexArray.h"

MeshComponent::MeshComponent(Actor* owner)
	:Component(owner)
	,mMesh(nullptr)
	,mTextureIndex(0)
{
	mOwner->GetGame()->GetRenderer()->AddMeshComp(this);
}

MeshComponent::~MeshComponent()
{
	mOwner->GetGame()->GetRenderer()->RemoveMeshComp(this);
}

void MeshComponent::Draw(Shader* shader)
{
	if (mMesh)
	{
		// Set the world transform
		// 월드 변환 행렬을 uniform으로 설정, 소유자 액터의 월드 변환 행렬을 직접 사용
		shader->SetMatrixUniform("uWorldTransform", 
			mOwner->GetWorldTransform());
		// Set specular power
		// 정반사 지수를 uniform으로 설정
		shader->SetFloatUniform("uSpecPower", mMesh->GetSpecPower());
		// Set the active texture
		// 활성화될 텍스처 설정
		Texture* t = mMesh->GetTexture(mTextureIndex);
		if (t)
		{
			t->SetActive();
		}
		// Set the mesh's vertex array as active
		// 활성화될 메시의 버텍스 배열 설정
		VertexArray* va = mMesh->GetVertexArray();
		va->SetActive();
		// Draw
		// 그린다
		glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
	}
}
