// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "SpriteComponent.h"
#include "Texture.h"
#include "Shader.h"
#include "Actor.h"
#include "Game.h"

SpriteComponent::SpriteComponent(Actor* owner, int drawOrder)
	:Component(owner)
	,mTexture(nullptr)
	,mDrawOrder(drawOrder)
	,mTexWidth(0)
	,mTexHeight(0)
{
	mOwner->GetGame()->AddSprite(this);
}

SpriteComponent::~SpriteComponent()
{
	mOwner->GetGame()->RemoveSprite(this);
}

void SpriteComponent::Draw(Shader* shader)
{
	if (mTexture)
	{
		// Scale the quad by the width/height of texture
		// 텍스처의 너비와 높이로 사각형을 스케일
		Matrix4 scaleMat = Matrix4::CreateScale(
			static_cast<float>(mTexWidth),
			static_cast<float>(mTexHeight),
			1.0f);
		
		Matrix4 world = scaleMat * mOwner->GetWorldTransform();
		
		// Since all sprites use the same shader/vertices,
		// the game first sets them active before any sprite draws
		
		
		// Set world transform
		// 월드 변환 행렬 설정
		shader->SetMatrixUniform("uWorldTransform", world);
		// Set current texture
		// 현재 텍스처 설정
		mTexture->SetActive();
		// Draw quad
		// 사각형 그리기
		glDrawElements(GL_TRIANGLES, // 그려야 할 폴리곤/프리미티브의 타입
			6, // 인덱스 버퍼에 있는 인덱스의 수
			GL_UNSIGNED_INT, // 각 인덱스의 타입
			nullptr); // nullptr
	}
}

void SpriteComponent::SetTexture(Texture* texture)
{
	mTexture = texture;
	// Set width/height
	mTexWidth = texture->GetWidth();
	mTexHeight = texture->GetHeight();
}
