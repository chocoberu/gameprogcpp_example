// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Renderer.h"
#include "Texture.h"
#include "Mesh.h"
#include <algorithm>
#include "Shader.h"
#include "VertexArray.h"
#include "SpriteComponent.h"
#include "MeshComponent.h"
#include <GL/glew.h>

Renderer::Renderer(Game* game)
	:mGame(game)
	,mSpriteShader(nullptr)
	,mMeshShader(nullptr)
{
}

Renderer::~Renderer()
{
}

bool Renderer::Initialize(float screenWidth, float screenHeight)
{
	mScreenWidth = screenWidth;
	mScreenHeight = screenHeight;

	// Set OpenGL attributes
	// OpenGL 속성 설정
	// Use the core OpenGL profile
	// 코어 OpenGL 프로필 사용
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	// Specify version 3.3
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	// Request a color buffer with 8-bits per RGBA channel
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24); // 깊이 버퍼 속성 설정
	// Enable double buffering
	// 더블 버퍼링 활성화
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	// Force OpenGL to use hardware acceleration
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	mWindow = SDL_CreateWindow("Game Programming in C++ (Chapter 6)", 100, 100,
		static_cast<int>(mScreenWidth), static_cast<int>(mScreenHeight), SDL_WINDOW_OPENGL);
	if (!mWindow)
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	// Create an OpenGL context
	mContext = SDL_GL_CreateContext(mWindow);

	// Initialize GLEW
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		SDL_Log("Failed to initialize GLEW.");
		return false;
	}

	// On some platforms, GLEW will emit a benign error code,
	// so clear it
	glGetError();

	// Make sure we can create/compile shaders
	if (!LoadShaders())
	{
		SDL_Log("Failed to load shaders.");
		return false;
	}

	// Create quad for drawing sprites
	CreateSpriteVerts();

	return true;
}

void Renderer::Shutdown()
{
	delete mSpriteVerts;
	mSpriteShader->Unload();
	delete mSpriteShader;
	mMeshShader->Unload();
	delete mMeshShader;
	SDL_GL_DeleteContext(mContext);
	SDL_DestroyWindow(mWindow);
}

void Renderer::UnloadData()
{
	// Destroy textures
	for (auto i : mTextures)
	{
		i.second->Unload();
		delete i.second;
	}
	mTextures.clear();

	// Destroy meshes
	for (auto i : mMeshes)
	{
		i.second->Unload();
		delete i.second;
	}
	mMeshes.clear();
	/*for (auto i : mBasicMeshes)
	{
		i.second->Unload();
		delete i.second;
	}*/
}

void Renderer::Draw()
{
	// Set the clear color to light grey
	// 화면 클리어
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	// Clear the color buffer, 색상 버퍼와 깊이 버퍼 초기화
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

	// Draw mesh components
	// Enable depth buffering/disable alpha blend
	// 메시 컴포넌트 그리기
	glEnable(GL_DEPTH_TEST); // 깊이 버퍼 활성화(z 버퍼링 활성화)
	glDisable(GL_BLEND); // 블렌드 비활성화
	
	// 6.1
	// Phong 셰이더 메시 컴포넌트 그리기
	mMeshShader->SetActive();
	//mMapShader["Phong"]->SetActive();
	// Update view-projection matrix
	// 뷰-투영 행렬 업데이트
	mMeshShader->SetMatrixUniform("uViewProj", mView * mProjection);
	//mMapShader["Phong"]->SetMatrixUniform("uViewProj", mView * mProjection);
	// Update lighting uniforms
	// 라이팅 uniform 업데이트
	SetLightUniforms(mMeshShader);
	//SetLightUniforms(mMapShader["Phong"]);
	for (auto mc : mMeshComps) // 렌더러의 메시 컴포넌트들을 그린다
	{
	
		mc->Draw(mMeshShader); // 메시 셰이더를 이용해서 그린다
	}

	// Draw all sprite components
	// Disable depth buffering
	glDisable(GL_DEPTH_TEST);
	// Enable alpha blending on the color buffer
	glEnable(GL_BLEND);
	glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

	// Set shader/vao as active
	mSpriteShader->SetActive();
	mSpriteVerts->SetActive();
	for (auto sprite : mSprites)
	{
		sprite->Draw(mSpriteShader); // 스프라이프 셰이더를 이용해서 그린다
	}

	// Swap the buffers
	SDL_GL_SwapWindow(mWindow);
}

void Renderer::AddSprite(SpriteComponent* sprite)
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

void Renderer::RemoveSprite(SpriteComponent* sprite)
{
	auto iter = std::find(mSprites.begin(), mSprites.end(), sprite);
	mSprites.erase(iter);
}

void Renderer::AddMeshComp(MeshComponent* mesh)
{
	mMeshComps.emplace_back(mesh);
}

void Renderer::RemoveMeshComp(MeshComponent* mesh)
{
	auto iter = std::find(mMeshComps.begin(), mMeshComps.end(), mesh);
	mMeshComps.erase(iter);
}

Texture* Renderer::GetTexture(const std::string& fileName)
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

Mesh* Renderer::GetMesh(const std::string & fileName)
{
	Mesh* m = nullptr;
	auto iter = mMeshes.find(fileName);
	if (iter != mMeshes.end())
	{
		m = iter->second;
	}
	else
	{
		m = new Mesh();
		if (m->Load(fileName, this))
		{
			mMeshes.emplace(fileName, m);
		}
		else
		{
			delete m;
			m = nullptr;
		}
	}
	return m;
}

bool Renderer::LoadShaders()
{
	// Create sprite shader
	mSpriteShader = new Shader();
	if (!mSpriteShader->Load("Shaders/Sprite.vert", "Shaders/Sprite.frag"))
	{
		return false;
	}

	mSpriteShader->SetActive();
	// Set the view-projection matrix
	Matrix4 viewProj = Matrix4::CreateSimpleViewProj(mScreenWidth, mScreenHeight);
	mSpriteShader->SetMatrixUniform("uViewProj", viewProj);

	// Create basic mesh shader
	mMeshShader = new Shader();
	if (!mMeshShader->Load("Shaders/Phong.vert", "Shaders/Phong.frag"))
	{
		return false;
	}

	mMeshShader->SetActive();
	// Set the view-projection matrix
	// 뷰-투영 행렬 설정
	mView = Matrix4::CreateLookAt(Vector3::Zero, // 카메라 위치
		Vector3::UnitX, // 타깃 위치
		Vector3::UnitZ); // 상향 벡터
	mProjection = Matrix4::CreatePerspectiveFOV(Math::ToRadians(70.0f), // 수평 FOV
		mScreenWidth, // 뷰의 너비
		mScreenHeight, // 뷰의 높이
		25.0f, // 가까운 평면과의 거리
		10000.0f); // 먼 평면과의 거리
	mMeshShader->SetMatrixUniform("uViewProj", mView * mProjection); // 뷰-투영 행렬을 uniform으로 설정

	return true;
}

void Renderer::CreateSpriteVerts()
{
	float vertices[] = {
		-0.5f, 0.5f, 0.f, 0.f, 0.f, 0.0f, 0.f, 0.f, // top left
		0.5f, 0.5f, 0.f, 0.f, 0.f, 0.0f, 1.f, 0.f, // top right
		0.5f,-0.5f, 0.f, 0.f, 0.f, 0.0f, 1.f, 1.f, // bottom right
		-0.5f,-0.5f, 0.f, 0.f, 0.f, 0.0f, 0.f, 1.f  // bottom left
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	mSpriteVerts = new VertexArray(vertices, 4, indices, 6);
}

void Renderer::SetLightUniforms(Shader* shader)
{
	// Camera position is from inverted view
	// 카메라 위치는 인버트된 뷰 행렬(역행렬)에서 얻을 수 있다.
	Matrix4 invView = mView;
	invView.Invert();
	shader->SetVectorUniform("uCameraPos", invView.GetTranslation());
	// Ambient light
	// 주변광
	shader->SetVectorUniform("uAmbientLight", mAmbientLight);
	// Directional light
	// 방향광
	shader->SetVectorUniform("uDirLight.mDirection",
		mDirLight.mDirection);
	shader->SetVectorUniform("uDirLight.mDiffuseColor",
		mDirLight.mDiffuseColor);
	shader->SetVectorUniform("uDirLight.mSpecColor",
		mDirLight.mSpecColor);
	
	shader->SetVectorUniform("uPointLight[0].mPosition", mPointLight[0].mPosition);
	shader->SetVectorUniform("uPointLight[0].mDiffuseColor", mPointLight[0].mDiffuseColor);
	shader->SetVectorUniform("uPointLight[0].mSpecColor", mPointLight[0].mSpecColor);

	shader->SetVectorUniform("uPointLight[1].mPosition", mPointLight[1].mPosition);
	shader->SetVectorUniform("uPointLight[1].mDiffuseColor", mPointLight[1].mDiffuseColor);
	shader->SetVectorUniform("uPointLight[1].mSpecColor", mPointLight[1].mSpecColor);

	shader->SetVectorUniform("uPointLight[2].mPosition", mPointLight[2].mPosition);
	shader->SetVectorUniform("uPointLight[2].mDiffuseColor", mPointLight[2].mDiffuseColor);
	shader->SetVectorUniform("uPointLight[2].mSpecColor", mPointLight[2].mSpecColor);

	shader->SetVectorUniform("uPointLight[3].mPosition", mPointLight[3].mPosition);
	shader->SetVectorUniform("uPointLight[3].mDiffuseColor", mPointLight[3].mDiffuseColor);
	shader->SetVectorUniform("uPointLight[3].mSpecColor", mPointLight[3].mSpecColor);
}
