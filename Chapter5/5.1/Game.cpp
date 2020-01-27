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
	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) != 0) // SDL ���̺귯�� �ʱ�ȭ 
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}
	
	// Set OpenGL attributes
	// Use the core OpenGL profile
	// OpenGL ������ �Ӽ� ����
	// �ھ� OpenGL ���������� ���
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	// Specify version 3.3
	// 3.3 �������� ����
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	// Request a color buffer with 8-bits per RGBA channel
	// RGBA ä�θ��� 8��Ʈ ũ���� ���� ���� ��û
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	// Enable double buffering
	// ���� ���۸� Ȱ��ȭ
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	// Force OpenGL to use hardware acceleration
	// OpenGL�� �ϵ���� ������ ����ϵ��� ����
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	

	mWindow = SDL_CreateWindow("Game Programming in C++ (Chapter 5)", 100, 100,
							   1024, 768, SDL_WINDOW_OPENGL); // �÷��׸� OpenGL �������� ����, OpenGL�� ����ϴ� ������� ����
	if (!mWindow)
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}
	
	// Create an OpenGL context
	// OpenGL ���ؽ�Ʈ ����, ���ؽ�Ʈ�� OpenGL�� �ν��ϴ� ��� ���³� ������Ʈ�� �����ϴ� OpenGL�� ����
	// OpenGL�� ���ؽ�Ʈ�� ���� ����, �ε�� �̹����� ��, �׸��� ���� �پ��� OpenGL ������Ʈ�� �����Ѵ�
	mContext = SDL_GL_CreateContext(mWindow);
	
	// Initialize GLEW
	// GLEW�� �ʱ�ȭ
	// OpenGL�� Ȯ�� �ý����� ����ϱ� ���� ���¼ҽ� ���̺귯��
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		SDL_Log("Failed to initialize GLEW.");
		return false;
	}
	
	// On some platforms, GLEW will emit a benign error code,
	// so clear it
	// �Ϻ� �÷������� GLEW�� ���� �ڵ带 ��������. �׷��Ƿ� �������� �����Ѵ�
	glGetError();
	
	// Make sure we can create/compile shaders
	// ���̴� �ε� (����/������)
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
	// ��Ÿ �ð� ���
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
	// ��� ���� ������Ʈ
	mUpdatingActors = true;
	for (auto actor : mActors)
	{
		actor->Update(deltaTime);
	}
	mUpdatingActors = false;

	// Move any pending actors to mActors
	// ��� ���� ���͸� ���� vector�� �߰�
	for (auto pending : mPendingActors)
	{
		pending->ComputeWorldTransform(); // ������� ������ ���� ��ȯ ��� ���
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
	// ȭ���� Ŭ�����ϰ� ����� �׸� �� ���۸� �����ϴ� ������ �ʿ�

	// Set the clear color to grey
	// ������ ȸ������ ����
	
	glClearColor(tempR, tempG, tempB, 1.0f);
	// Clear the color buffer
	// ���� ���� �ʱ�ȭ 
	glClear(GL_COLOR_BUFFER_BIT);
	
	// ����� �׸��� ����

	// Draw all sprite components
	// Enable alpha blending on the color buffer
	// ��� ��������Ʈ ������Ʈ�� �׸���
	// ���� ���� Ȱ��ȭ
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, // srcFactor = srcAlpha
		GL_ONE_MINUS_SRC_ALPHA); // dstFactor = 1 - srcAlpha
	
	// Set shader/vao as active
	// ��Ʈ������ ���̴��� ���ؽ� �迭 ��ü�� Ȱ��ȭ
	mSpriteShader->SetActive(); // ���̴� Ȱ��ȭ
	mSpriteVerts->SetActive(); // ���ؽ� �迭 ��ü Ȱ��ȭ
	for (auto sprite : mSprites) // ��� ��������Ʈ�� �׸���
	{
		sprite->Draw(mSpriteShader);
	}

	// Swap the buffers
	// ���۸� �����ؼ� ����� ����Ѵ�
	SDL_GL_SwapWindow(mWindow);
}

bool Game::LoadShaders()
{
	mSpriteShader = new Shader(); // ��������Ʈ�� �׸��µ� ���Ǵ� ���̴� ����
	if (!mSpriteShader->Load("Shaders/Sprite.vert", "Shaders/Sprite.frag"))
	{
		return false;
	}

	mSpriteShader->SetActive();
	// Set the view-projection matrix
	// ��-���� ����� ����
	Matrix4 viewProj = Matrix4::CreateSimpleViewProj(1024.f, 768.f);
	mSpriteShader->SetMatrixUniform("uViewProj", viewProj);
	return true;
}

void Game::CreateSpriteVerts()
{
	float vertices[] = { // ������ 2���� �ؽ�ó ��ǥ(u,v)
		-0.5f,  0.5f, 0.f, 0.f, 0.f, // top left ���� ���
		 0.5f,  0.5f, 0.f, 1.f, 0.f, // top right ������ ���
		 0.5f, -0.5f, 0.f, 1.f, 1.f, // bottom right ������ �ϴ�
		-0.5f, -0.5f, 0.f, 0.f, 1.f  // bottom left ���� �ϴ�
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
	SDL_GL_DeleteContext(mContext); // OpenGL ���ؽ�Ʈ �Ҹ�
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
