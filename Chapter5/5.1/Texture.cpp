// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Texture.h"
#include <SOIL/SOIL.h>
#include <GL/glew.h>
#include <SDL/SDL.h>

Texture::Texture()
:mTextureID(0)
,mWidth(0)
,mHeight(0)
{
	
}

Texture::~Texture()
{
	
}

bool Texture::Load(const std::string& fileName)
{
	int channels = 0;
	
	unsigned char* image = SOIL_load_image(fileName.c_str(), // 파일 이름
										   &mWidth, // 너비 저장
											&mHeight, // 높이 저장
											&channels, // 채널 수 저장
											SOIL_LOAD_AUTO); // 이미지 파일 타입 또는 자동
	
	if (image == nullptr) // 텍스처 이미지가 제대로 로드되지 않았다면
	{
		SDL_Log("SOIL failed to load image %s: %s", fileName.c_str(), SOIL_last_result());
		return false;
	}
	
	int format = GL_RGB;
	if (channels == 4) // 알파값이 있다면
	{
		format = GL_RGBA;
	}
	
	glGenTextures(1, &mTextureID); // OpenGL 텍스처 오브젝트 생성
	glBindTexture(GL_TEXTURE_2D, mTextureID); // 텍스처 활성화
	
	//원본 이미지 데이터를 텍스처 오브젝트에 복사
	glTexImage2D(GL_TEXTURE_2D, // 텍스처 타깃
		0, // LOD (지금은 0으로 지정)
		format, // OpenGL이 사용해야 되는 색상 포맷
		mWidth, // 텍스처의 너비
		mHeight, // 텍스처의 높이
		0, // 보더 - "이 값은 0이어야 한다"
		format, // 입력 데이터의 색상 포맷
		GL_UNSIGNED_BYTE, // 입력 데이터의 비트 깊이 (Unsigned byte는 8비트 채널을 지정)
		image); // 이미지 데이터의 포인터
	
	SOIL_free_image_data(image); // 원본 이미지 데이터 해제
	
	// Enable bilinear filtering
	// 이중선형 필터링 활성화
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	return true;
}

void Texture::Unload()
{
	glDeleteTextures(1, &mTextureID); // 텍스처 오브젝트 삭제
}

void Texture::SetActive()
{
	glBindTexture(GL_TEXTURE_2D, mTextureID);
}
