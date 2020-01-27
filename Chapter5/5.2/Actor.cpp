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
	// 컴포넌트들을 제거할 필요가 있다
	// 컴포넌트 소멸자는 RemoveComponent()를 호출하기 때문에 다른 루프 스타일이 필요
	while (!mComponents.empty())
	{
		delete mComponents.back();
	}
}

void Actor::Update(float deltaTime)
{
	if (mState == EActive) // State가 활동 상태라면
	{
		ComputeWorldTransform(); // 월드 행렬 계산

		UpdateComponents(deltaTime); // 액터의 컴포넌트 업데이트
		UpdateActor(deltaTime); // 액터 업데이트

		ComputeWorldTransform(); // 잠정적으로 액터가 변경되었다고 판단하여 다시 월드 행렬 계산
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

void Actor::ComputeWorldTransform() // 월드 변환 행렬을 계산
{
	if (mRecomputeWorldTransform) // 다시 계산할 필요가 있다면
	{
		mRecomputeWorldTransform = false; // bool 변수 초기화
		// Scale, then rotate, then translate
		// SRT 순으로 계산 (행 벡터로 계산하기 때문, 열 벡터라면 TRS)
		mWorldTransform = Matrix4::CreateScale(mScale);
		mWorldTransform *= Matrix4::CreateRotationZ(mRotation); // 2차원 회전시 z축 기준 회전만 필요
		mWorldTransform *= Matrix4::CreateTranslation(Vector3(mPosition.x, mPosition.y, 0.0f));

		// Inform components world transform updated
		// 컴포넌트에 세계 변환이 갱신됐다고 통지 (현재는 OnupdateWorldTransform() 구현 X)
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
