// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Actor.h"
#include "Game.h"
#include "Component.h"
#include <algorithm>

Actor::Actor(Game* game)
	:mState(EActive)
	,mPosition(Vector2::Zero)
	,mScale(1.0f)
	,mRotation(0.0f)
	,mGame(game)
	,mRecomputeWorldTransform(true)
{
	mGame->AddActor(this);
}

Actor::~Actor()
{
	mGame->RemoveActor(this);
	// Need to delete components
	// Because ~Component calls RemoveComponent, need a different style loop
	// ������Ʈ���� ������ �ʿ䰡 �ִ�
	// ������Ʈ �Ҹ��ڴ� RemoveComponent()�� ȣ���ϱ� ������ �ٸ� ���� ��Ÿ���� �ʿ�
	while (!mComponents.empty())
	{
		delete mComponents.back();
	}
}

void Actor::Update(float deltaTime)
{
	if (mState == EActive) // State�� Ȱ�� ���¶��
	{
		ComputeWorldTransform(); // ���� ��� ���

		UpdateComponents(deltaTime); // ������ ������Ʈ ������Ʈ
		UpdateActor(deltaTime); // ���� ������Ʈ

		ComputeWorldTransform(); // ���������� ���Ͱ� ����Ǿ��ٰ� �Ǵ��Ͽ� �ٽ� ���� ��� ���
	}
}

void Actor::UpdateComponents(float deltaTime)
{
	for (auto comp : mComponents)
	{
		comp->Update(deltaTime);
	}
}

void Actor::UpdateActor(float deltaTime)
{
}

void Actor::ProcessInput(const uint8_t* keyState)
{
	if (mState == EActive)
	{
		// First process input for components
		for (auto comp : mComponents)
		{
			comp->ProcessInput(keyState);
		}

		ActorInput(keyState);
	}
}

void Actor::ActorInput(const uint8_t* keyState)
{
}

void Actor::ComputeWorldTransform() // ���� ��ȯ ����� ���
{
	if (mRecomputeWorldTransform) // �ٽ� ����� �ʿ䰡 �ִٸ�
	{
		mRecomputeWorldTransform = false; // bool ���� �ʱ�ȭ
		// Scale, then rotate, then translate
		// SRT ������ ��� (�� ���ͷ� ����ϱ� ����, �� ���Ͷ�� TRS)
		mWorldTransform = Matrix4::CreateScale(mScale);
		mWorldTransform *= Matrix4::CreateRotationZ(mRotation); // 2���� ȸ���� z�� ���� ȸ���� �ʿ�
		mWorldTransform *= Matrix4::CreateTranslation(Vector3(mPosition.x, mPosition.y, 0.0f));

		// Inform components world transform updated
		// ������Ʈ�� ���� ��ȯ�� ���ŵƴٰ� ���� (����� OnupdateWorldTransform() ���� X)
		for (auto comp : mComponents)
		{
			comp->OnUpdateWorldTransform();
		}
	}
}

void Actor::AddComponent(Component* component)
{
	// Find the insertion point in the sorted vector
	// (The first element with a order higher than me)
	int myOrder = component->GetUpdateOrder();
	auto iter = mComponents.begin();
	for (;
		iter != mComponents.end();
		++iter)
	{
		if (myOrder < (*iter)->GetUpdateOrder())
		{
			break;
		}
	}

	// Inserts element before position of iterator
	mComponents.insert(iter, component);
}

void Actor::RemoveComponent(Component* component)
{
	auto iter = std::find(mComponents.begin(), mComponents.end(), component);
	if (iter != mComponents.end())
	{
		mComponents.erase(iter);
	}
}
