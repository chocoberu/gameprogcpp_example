// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include <vector>
#include <string>

class Mesh
{
public:
	Mesh();
	~Mesh();
	// Load/unload mesh
	// 메시 로드/언로드
	bool Load(const std::string& fileName, class Renderer* renderer);
	void Unload();
	// Get the vertex array associated with this mesh
	// 이 메시에 해당하는 버텍스 배열을 얻는다
	class VertexArray* GetVertexArray() { return mVertexArray; }
	// Get a texture from specified index
	// 특정 인덱스에 해당하는 텍스처를 얻는다
	class Texture* GetTexture(size_t index);
	// Get name of shader
	// 셰이더의 이름을 얻는다
	const std::string& GetShaderName() const { return mShaderName; }
	// Get object space bounding sphere radius
	// 오브젝트 공간 바운딩 구체 반지름을 얻는다
	float GetRadius() const { return mRadius; }
	// Get specular power of mesh
	// 
	float GetSpecPower() const { return mSpecPower; }
private:
	// Textures associated with this mesh
	// 이 메시에서 사용되는 텍스처들
	std::vector<class Texture*> mTextures;
	// Vertex array associated with this mesh
	// 메시의 버텍스 배열
	class VertexArray* mVertexArray;
	// Name of shader specified by mesh
	// 메시가 지정한 셰이더 이름
	std::string mShaderName;
	// Stores object space bounding sphere radius
	// 오브젝트 공간 바운딩 구체의 반지름값 저장
	float mRadius;
	// Specular power of surface
	// 메시 표면의 정반사 지수
	float mSpecPower;
};