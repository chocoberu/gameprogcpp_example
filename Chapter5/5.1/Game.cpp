// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Game.h"
#include <GL/glew.h>
#include "Texture.h"
#include "VertexArray.h"
#include "Shader.h"
#include <algorithm>
#include "Actor.h"
#include "SpriteComponent.h"
#include "Actor.h"
#include "Ship.h"
#include "Asteroid.h"
#include "Random.h"

Game::Game()
:mWindow(nullptr)
,mSpriteShader(nullptr)
,mIsRunning(true)
,mUpdatingActors(false)
{
	
}

bool Game::Initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) != 0) // SDL 라이브러리 초기화 
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}
	
	// Set OpenGL attributes
	// Use the core OpenGL profile
	// OpenGL 윈도우 속성 설정
	// 코어 OpenGL 프로파일을 사용
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	// Specify version 3.3
	// 3.3 버전으로 지정
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	// Request a color buffer with 8-bits per RGBA channel
	// RGBA 채널마다 8비트 크기인 색상 버퍼 요청
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	// Enable double buffering
	// 더블 버퍼링 활성화
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	// Force OpenGL to use hardware acceleration
	// OpenGL이 하드웨어 가속을 사용하도록 강제
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	

	mWindow = SDL_CreateWindow("Game Programming in C++ (Chapter 5)", 100, 100,
							   1024, 768, SDL_WINDOW_OPENGL); // 플래그를 OpenGL 관련으로 설정, OpenGL을 사용하는 윈도우로 생성
	if (!mWindow)
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}
	
	// Create an OpenGL context
	// OpenGL 콘텍스트 생성, 콘텍스트는 OpenGL이 인식하는 모든 상태나 오브젝트를 포함하는 OpenGL의 세계
	// OpenGL의 콘텍스트는 색상 깊이, 로드된 이미지나 모델, 그리고 여러 다양한 OpenGL 오브젝트를 포함한다
	mContext = SDL_GL_CreateContext(mWindow);
	
	// Initialize GLEW
	// GLEW를 초기화
	// OpenGL의 확장 시스템을 사용하기 위한 오픈소스 라이브러리
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		SDL_Log("Failed to initialize GLEW.");
		return false;
	}
	
	// On some platforms, GLEW will emit a benign error code,
	// so clear it
	// 일부 플랫폼에서 GLEW은 에러 코드를 내보낸다. 그러므로 에러값을 제거한다
	glGetError();
	
	// Make sure we can create/compile shaders
	// 셰이더 로드 (생성/컴파일)
	if (!LoadShaders())
	{
		SDL_Log("Failed to load shaders.");
		return false;
	}

	// Create quad for drawing sprites
	CreateSpriteVerts();

	LoadData();

	mTicksCount = SDL_GetTicks();
	
	tempR = tempG = tempB = 0.86f;
	return true;
}

void Game::RunLoop()
{
	while (mIsRunning)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::ProcessInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
				mIsRunning = false;
				break;
		}
	}
	
	const Uint8* keyState = SDL_GetKeyboardState(NULL);
	if (keyState[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}

	mUpdatingActors = true;
	for (auto actor : mActors)
	{
		actor->ProcessInput(keyState);
	}
	mUpdatingActors = false;
}

void Game::UpdateGame()
{
	// Compute delta time
	// Wait until 16ms has elapsed since last frame
	// 델타 시간 계산
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
		;

	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}
	mTicksCount = SDL_GetTicks();
	tempR += deltaTime;
	tempG += deltaTime;
	tempG += deltaTime;

	if (tempR > 1.0f)
		tempR = 0;
	if (tempG > 1.0f)
		tempG = 0;
	if (tempB > 1.0f)
		tempB = 0;

	// Update all actors
	// 모든 액터 업데이트
	mUpdatingActors = true;
	for (auto actor : mActors)
	{
		actor->Update(deltaTime);
	}
	mUpdatingActors = false;

	// Move any pending actors to mActors
	// 대기 중인 액터를 액터 vector에 추가
	for (auto pending : mPendingActors)
	{
		pending->ComputeWorldTransform(); // 대기중인 액터의 월드 변환 행렬 계산
		mActors.emplace_back(pending);
	}
	mPendingActors.clear();

	// Add any dead actors to a temp vector
	std::vector<Actor*> deadActors;
	for (auto actor : mActors)
	{
		if (actor->GetState() == Actor::EDead)
		{
			deadActors.emplace_back(actor);
		}
	}

	// Delete dead actors (which removes them from mActors)
	for (auto actor : deadActors)
	{
		delete actor;
	}
}

void Game::GenerateOutput()
{
	// 화면을 클리어하고 장면을 그린 뒤 버퍼를 스왑하는 과정이 필요

	// Set the clear color to grey
	// 색상을 회색으로 설정
	
	glClearColor(tempR, tempG, tempB, 1.0f);
	// Clear the color buffer
	// 색상 버퍼 초기화 
	glClear(GL_COLOR_BUFFER_BIT);
	
	// 장면을 그리는 과정

	// Draw all sprite components
	// Enable alpha blending on the color buffer
	// 모든 스프라이트 컴포넌트를 그린다
	// 알파 블렌딩 활성화
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, // srcFactor = srcAlpha
		GL_ONE_MINUS_SRC_ALPHA); // dstFactor = 1 - srcAlpha
	
	// Set shader/vao as active
	// 스트라이프 셰이더와 버텍스 배열 객체를 활성화
	mSpriteShader->SetActive(); // 셰이더 활성화
	mSpriteVerts->SetActive(); // 버텍스 배열 객체 활성화
	for (auto sprite : mSprites) // 모든 스프라이트를 그린다
	{
		sprite->Draw(mSpriteShader);
	}

	// Swap the buffers
	// 버퍼를 스왑해서 장면을 출력한다
	SDL_GL_SwapWindow(mWindow);
}

bool Game::LoadShaders()
{
	mSpriteShader = new Shader(); // 스프라이트를 그리는데 사용되는 셰이더 생성
	if (!mSpriteShader->Load("Shaders/Sprite.vert", "Shaders/Sprite.frag"))
	{
		return false;
	}

	mSpriteShader->SetActive();
	// Set the view-projection matrix
	// 뷰-투영 행렬을 설정
	Matrix4 viewProj = Matrix4::CreateSimpleViewProj(1024.f, 768.f);
	mSpriteShader->SetMatrixUniform("uViewProj", viewProj);
	return true;
}

void Game::CreateSpriteVerts()
{
	float vertices[] = { // 마지막 2개는 텍스처 좌표(u,v)
		-0.5f,  0.5f, 0.f, 0.f, 0.f, // top left 왼쪽 상단
		 0.5f,  0.5f, 0.f, 1.f, 0.f, // top right 오른쪽 상단
		 0.5f, -0.5f, 0.f, 1.f, 1.f, // bottom right 오른쪽 하단
		-0.5f, -0.5f, 0.f, 0.f, 1.f  // bottom left 왼쪽 하단
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	mSpriteVerts = new VertexArray(vertices, 4, indices, 6);
}

void Game::LoadData()
{
	// Create player's ship
	mShip = new Ship(this);
	mShip->SetRotation(Math::PiOver2);

	// Create asteroids
	const int numAsteroids = 20;
	for (int i = 0; i < numAsteroids; i++)
	{
		new Asteroid(this);
	}
}

void Game::UnloadData()
{
	// Delete actors
	// Because ~Actor calls RemoveActor, have to use a different style loop
	while (!mActors.empty())
	{
		delete mActors.back();
	}

	// Destroy textures
	for (auto i : mTextures)
	{
		i.second->Unload();
		delete i.second;
	}
	mTextures.clear();
}

Texture* Game::GetTexture(const std::string& fileName)
{
	Texture* tex = nullptr;
	auto iter = mTextures.find(fileName);
	if (iter != mTextures.end())
	{
		tex = iter->second;
	}
	else
	{
		tex = new Texture();
		if (tex->Load(fileName))
		{
			mTextures.emplace(fileName, tex);
		}
		else
		{
			delete tex;
			tex = nullptr;
		}
	}
	return tex;
}

void Game::AddAsteroid(Asteroid* ast)
{
	mAsteroids.emplace_back(ast);
}

void Game::RemoveAsteroid(Asteroid* ast)
{
	auto iter = std::find(mAsteroids.begin(),
		mAsteroids.end(), ast);
	if (iter != mAsteroids.end())
	{
		mAsteroids.erase(iter);
	}
}

void Game::Shutdown()
{
	UnloadData();
	delete mSpriteVerts;
	mSpriteShader->Unload();
	delete mSpriteShader;
	SDL_GL_DeleteContext(mContext); // OpenGL 콘텍스트 소멸
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}

void Game::AddActor(Actor* actor)
{
	// If we're updating actors, need to add to pending
	if (mUpdatingActors)
	{
		mPendingActors.emplace_back(actor);
	}
	else
	{
		mActors.emplace_back(actor);
	}
}

void Game::RemoveActor(Actor* actor)
{
	// Is it in pending actors?
	auto iter = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
	if (iter != mPendingActors.end())
	{
		// Swap to end of vector and pop off (avoid erase copies)
		std::iter_swap(iter, mPendingActors.end() - 1);
		mPendingActors.pop_back();
	}

	// Is it in actors?
	iter = std::find(mActors.begin(), mActors.end(), actor);
	if (iter != mActors.end())
	{
		// Swap to end of vector and pop off (avoid erase copies)
		std::iter_swap(iter, mActors.end() - 1);
		mActors.pop_back();
	}
}

void Game::AddSprite(SpriteComponent* sprite)
{
	// Find the insertion point in the sorted vector
	// (The first element with a higher draw order than me)
	int myDrawOrder = sprite->GetDrawOrder();
	auto iter = mSprites.begin();
	for (;
		iter != mSprites.end();
		++iter)
	{
		if (myDrawOrder < (*iter)->GetDrawOrder())
		{
			break;
		}
	}

	// Inserts element before position of iterator
	mSprites.insert(iter, sprite);
}

void Game::RemoveSprite(SpriteComponent* sprite)
{
	auto iter = std::find(mSprites.begin(), mSprites.end(), sprite);
	mSprites.erase(iter);
}
