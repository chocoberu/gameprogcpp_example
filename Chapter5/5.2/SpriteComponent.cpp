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
		// �ؽ�ó�� �ʺ�� ���̷� �簢���� ������
		Matrix4 scaleMat = Matrix4::CreateScale(
			static_cast<float>(mTexWidth),
			static_cast<float>(mTexHeight),
			1.0f);
		
		Matrix4 world = scaleMat * mOwner->GetWorldTransform();
		
		// Since all sprites use the same shader/vertices,
		// the game first sets them active before any sprite draws
		
		
		// Set world transform
		// ���� ��ȯ ��� ����
		shader->SetMatrixUniform("uWorldTransform", world);
		// Set current texture
		// ���� �ؽ�ó ����
		mTexture->SetActive();
		// Draw quad
		// �簢�� �׸���
		glDrawElements(GL_TRIANGLES, // �׷��� �� ������/������Ƽ���� Ÿ��
			6, // �ε��� ���ۿ� �ִ� �ε����� ��
			GL_UNSIGNED_INT, // �� �ε����� Ÿ��
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
